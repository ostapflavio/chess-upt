#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Connect to localhost
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    printf("Connected to server. You are playing White.\n");

    while (1) {
        // Prompt for White's move
        printf("Your move (White): ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        // Send move to server
        send(sock, buffer, strlen(buffer), 0);

        // Wait for Black's move
        read(sock, buffer, BUFFER_SIZE);
        printf("Black played: %s\n", buffer);
    }

    close(sock);
    return 0;
}
