#include <stdio.h>
    // void perror(const char *s);
#include <stdlib.h>
    // void exit(int status);
#include <sys/types.h>
    // int socket(int domain, int type, int protocol);
#include <sys/socket.h>
    // int socket(int domain, int type, int protocol);


int main() {
    // 1. Create a socket
    int sockfd = socket(                    // Create a socket
                   AF_INET,                 //   IPv4 Internet protocols
                   SOCK_STREAM,             //   Stream socket type (TCP)
                   0);                      //   Default protocol
    if (sockfd < 0) {
        perror("ERROR creating socket");
        exit(1);
    }

    // 2. Bind socket to a port
    // 3. Wait for client to connect
    // 4. Send Hello, World
    printf("Server: With socket\n");
    return 0;
}

/*
# This: server-with-socket.c
# Prev: server-initial.c
# Next: server-bind-to-port.c

# Build & run:
gcc server-with-socket.c -o server && ./server
*/

