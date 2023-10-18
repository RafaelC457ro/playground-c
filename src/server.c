#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

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
    if (listen(server_fd, 3) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1)
    {
        // Accept incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept failed");
            exit(EXIT_FAILURE);
        }

        printf("New client connected.\n");

        // Receive and process messages from the client
        while (1)
        {
            int valread = read(new_socket, buffer, BUFFER_SIZE);
            if (valread <= 0)
            {
                // Client closed the connection or an error occurred
                printf("Client disconnected.\n");
                break;
            }

            // Print the received message
            printf("Received: %s", buffer);

            // Send "ok" back to the client
            char *response = "ok\n";
            send(new_socket, response, strlen(response), 0);

            // Clear the buffer for the next message
            memset(buffer, 0, BUFFER_SIZE);
        }

        // Close the socket for this client and wait for the next connection
        close(new_socket);
    }

    return 0;
}