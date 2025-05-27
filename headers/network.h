#ifndef NET_H
#define NET_H

#include <stddef.h>   /* size_t */

/* Create a listening socket on @port, accept exactly one connection,
 * close the listening socket, and return the connected socket FD.
 * Aborts the program on any system-call failure.                       */
int start_server(int port);

/* Connect to tcp://@addr:@port and return the connected socket FD.
 * Aborts the program on any system-call failure.                       */
int connect_to_server(const char *addr, int port);

/* Send exactly @len bytes from @buf. Returns the number of bytes sent
 * (== len) on success, –1 on error.                                    */
int sendall(int sock, const char *buf, size_t len);

/* Receive into @buf until either a NUL byte is seen or the buffer has
 * room for only one more byte (so buf[len-1] is always '\0').
 * Returns the number of bytes read (including the terminating NUL),
 *   0 if the peer closed the connection cleanly, or –1 on error.       */
int recvall(int sock, char *buf, size_t len);

#endif /* NET_H */
