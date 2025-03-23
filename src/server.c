// server side implementation of chess game 
#include <arpa/inet.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define PORT 4242
#define MAX_QUEUE 10


int main() {
    // server and client side file descriptors 
    int serverfd = socket(AF_INET, SOCK_STREAM, 0);  
    int clientfd;                                    
    
    // create the structure to represent the server side 
    struct sockaddr_in sa; 

    // create the structure to represent the client connection 
    struct sockaddr_in client_addr; 
    socklen_t addr_size; 

    // prepare the address and port for the server socket 
    memset(&sa, 0, sizeof(sa));                         // allocate memory
    sa.sin_family = AF_INET;                            // we work with IPv4 addresses 
    sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);        // bind localhost as our address
    sa.sin_port = htons(PORT);                          // bind our port number 

    serverfd     =  socket(sa.sin_family, SOCK_STREAM, 0); 
    int status   =  bind(serverfd, (struct sockaddr *)&sa, sizeof(sa)); 
    if(status != 0) { 
        fprintf(stderr, "We could not bind our IP address to our socket!"); 
        exit(EXIT_FAILURE); 
    }

    printf("Successfully binded! Now we are listening to any incoming connections... \n");
    listen(serverfd, MAX_QUEUE); 
    

    // accept incoming connection
    addr_size = sizeof(client_addr); 
    clientfd = accept(serverfd, (struct sockaddr *)&client_addr, &addr_size); 
    if (clientfd == -1) {
        fprintf(stderr, "accept: %s\n", strerror(errno)); 
        exit(EXIT_FAILURE);
    }
    printf("New connection! Socket fd: %d, client: %d \n", serverfd, clientfd); 
}
