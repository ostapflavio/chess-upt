#ifndef SERVER_H
#define SERVER_H

/* Starts a blocking TCP chess server on the given port.
 * Returns the exit status of the internal game loop. */
int run_server(int port);

#endif /* SERVER_H */