#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 12345
#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024

void *handle_client(void *arg)
{
    int client_socket = *((int *)arg);
    char buffer[BUFFER_SIZE] = {0};

    while (1)
    {
        int valread = read(client_socket, buffer, BUFFER_SIZE);
        if (valread <= 0)
        {
            // Client closed the connection or an error occurred
            printf("Client disconnected.\n");
            break;
        }

        // Print the received message
        printf("Received from client %d: %s", client_socket, buffer);

        char *response = "\n\nOK\n";
        send(client_socket, response, strlen(response), 0);

        // Clear the buffer for the next message
        memset(buffer, 0, BUFFER_SIZE);
    }

    // Close the client socket
    close(client_socket);

    return NULL;
}

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    pthread_t tid[MAX_CLIENTS]; // Thread IDs for each client

    // Create TCP socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to the specified address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, MAX_CLIENTS) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    int client_count = 0;
    while (1)
    {
        // Accept incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept failed");
            continue;
        }

        printf("New client connected.\n");

        // Create a new thread for the client
        if (pthread_create(&tid[client_count], NULL, handle_client, (void *)&new_socket) != 0)
        {
            perror("pthread_create failed");
            close(new_socket);
        }

        // Increment the client count, reset if it reaches MAX_CLIENTS
        client_count = (client_count + 1) % MAX_CLIENTS;
    }

    // Close the server socket
    close(server_fd);

    return 0;
}
