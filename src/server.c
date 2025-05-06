#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 1);
    printf("Waiting for client...\n");

    new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
    printf("Client connected. You are playing Black.\n");

    while (1) {
        // Wait for White's move
        read(new_socket, buffer, BUFFER_SIZE);
        printf("White played: %s\n", buffer);
        // If checkmate received -- gameover here
        printf("it's checkmate, you lost!\n");

        // Prompt for Black's move
        printf("Your move (Black): ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0; // remove newline

        // Send move to client
        send(new_socket, buffer, strlen(buffer), 0);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}

