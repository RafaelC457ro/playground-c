#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "queue.h"

#define PORT 8080
#define SERVER_BACKLOG 1024
#define BUFFER_SIZE 1024
#define MAX_THREADS 4

pthread_t thread_pool[MAX_THREADS];
Queue *queue = NULL;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *thread_pool_worker(void *args);

int main(int argc, char **argv)
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    queue = init_queue();

    // Create TCP socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // create thread pool
    for (int i = 0; i < MAX_THREADS; i++)
    {
        if (pthread_create(&thread_pool[i], NULL, thread_pool_worker, NULL))
        {
            perror("pthread_create() error");
            exit(1);
        }
    }

    // bind
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // listen
    if (listen(server_fd, SERVER_BACKLOG) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1)
    {
        // accept
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        // add to queue
        pthread_mutex_lock(&lock);
        enqueue(queue, new_socket);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&lock);
    }

    // close
    close(server_fd);

    return 0;
}

void client_handler(int client_socket)
{

    // Receive and process messages from the client
    while (1)
    {
        char buffer[BUFFER_SIZE] = {0};
        int valread = read(client_socket, buffer, BUFFER_SIZE);
        if (valread <= 0)
        {
            break;
        }
        printf("Received: %s", buffer);

        // send back OK
        char *response = "OK\n\n";
        send(client_socket, response, strlen(response), 0);
        memset(buffer, 0, BUFFER_SIZE);
    }

    printf("Client disconnected.\n");

    // Close the client socket
    close(client_socket);
}

void *thread_pool_worker(void *args)
{
    int client_socket;

    while (1)
    {
        pthread_mutex_lock(&lock);
        while (is_empty(queue))
        {
            pthread_cond_wait(&cond, &lock);
        }
        client_socket = dequeue(queue);
        pthread_mutex_unlock(&lock);

        printf("Handling client %d\n", client_socket);
        client_handler(client_socket);
    }

    return NULL;
}