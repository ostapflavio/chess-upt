#include "client.h"
#include "net.h"            /* connect_to_server() */
#include "game_loop.h"      /* game_loop(), BLACK */
#include <stdio.h>
#include <stdlib.h>

int run_client(const char *addr, int port)
{
    printf("Connecting to %s:%d …\n", addr, port);
    int sock = connect_to_server(addr, port);
    puts("Connected!");
    return game_loop(sock, BLACK);
}

int main(int argc, char **argv)
{
    const char *addr = (argc > 1) ? argv[1] : "127.0.0.1";
    int  port       = (argc > 2) ? atoi(argv[2]) : 12345;
    if (port <= 0 || port > 65535) {
        fprintf(stderr, "Invalid port number.\n");
        return 1;
    }
    return run_client(addr, port);
}