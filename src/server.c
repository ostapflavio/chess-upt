#include "server.h"
#include "net.h"            /* start_server() */
#include "game_loop.h"      /* game_loop(), WHITE */
#include <stdio.h>
#include <stdlib.h>

int run_server(int port)
{
    printf("Waiting for a connection on port %d …\n", port);
    int sock = start_server(port);          /* blocks until a client connects */
    puts("Opponent connected.");
    return game_loop(sock, WHITE);          /* this blocks until game ends    */
}

int main(int argc, char **argv)
{
    int port = (argc > 1) ? atoi(argv[1]) : 12345;
    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Invalid port number.\n");
        return 1;
    }
    return run_server(port);
}