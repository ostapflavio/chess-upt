#include "network.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define DIE(msg)  do { perror(msg); exit(EXIT_FAILURE); } while (0)

/*───────────────────────────────────────────────────────────────────*/

int start_server(int port)
{
    int lsock = socket(AF_INET, SOCK_STREAM, 0);
    if (lsock < 0) DIE("socket");

    int opt = 1;
    if (setsockopt(lsock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        DIE("setsockopt");

    struct sockaddr_in addr = {0};
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port        = htons((uint16_t)port);

    if (bind(lsock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        DIE("bind");

    if (listen(lsock, 1) < 0)
        DIE("listen");

    int csock = accept(lsock, NULL, NULL);
    if (csock < 0) DIE("accept");

    close(lsock);                       /* no longer need the listener */
    return csock;
}

/*───────────────────────────────────────────────────────────────────*/

int connect_to_server(const char *addr, int port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) DIE("socket");

    struct sockaddr_in a = {0};
    a.sin_family = AF_INET;
    a.sin_port   = htons((uint16_t)port);

    if (inet_pton(AF_INET, addr, &a.sin_addr) <= 0)
        DIE("inet_pton");

    if (connect(sock, (struct sockaddr *)&a, sizeof(a)) < 0)
        DIE("connect");

    return sock;
}

/*───────────────────────────────────────────────────────────────────*/

int sendall(int sock, const char *buf, size_t len)
{
    size_t sent = 0;
    while (sent < len) {
        ssize_t n = send(sock, buf + sent, len - sent, 0);
        if (n < 0) {
            if (errno == EINTR) continue;   /* retry on signal */
            return -1;                      /* fatal error     */
        }
        sent += (size_t)n;
    }
    return (int)sent;
}

/*───────────────────────────────────────────────────────────────────*/

int recvall(int sock, char *buf, size_t len)
{
    if (len == 0) return -1;
    size_t got = 0;

    /* leave room for terminating NUL */
    while (got < len - 1) {
        ssize_t n = recv(sock, buf + got, 1, 0);   /* one byte at a time */
        if (n < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        if (n == 0) {              /* peer closed socket */
            buf[got] = '\0';
            return 0;
        }
        if (buf[got] == '\0') {    /* we hit the NUL terminator */
            return (int)(got + 1);
        }
        got += (size_t)n;
    }

    buf[got] = '\0';
    return (int)got;
}
