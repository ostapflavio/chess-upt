#ifndef CLIENT_H
#define CLIENT_H

/* Connects to a TCP chess server at addr:port.
 * Returns the exit status of the internal game loop. */
int run_client(const char *addr, int port);

#endif /* CLIENT_H */