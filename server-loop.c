#include <stdio.h>
    // void perror(const char *s);
#include <stdlib.h>
    // void exit(int status);
    // int atoi(const char *nptr);
#include <sys/types.h>
#include <sys/socket.h>
    // int socket(int domain, int type, int protocol);
    // int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    // int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
#include <strings.h>
    // void bzero(void *s, size_t n);
#include <netinet/in.h>
    // struct sockaddr_in
#include <arpa/inet.h>
    // uint16_t htons(uint16_t hostshort);
#include <unistd.h>
    // ssize_t write(int fd, const void *buf, size_t count);

int main(int argc, char *argv[])
{
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
    int portno = atoi(argv[1]);             // Get port number from argument

    struct sockaddr_in serv_addr;           // Internet address with port
    bzero( (char*) &serv_addr,              //   Initialize struct to zeros '\0'
           sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;         //   IPv4 Internet protocols
    serv_addr.sin_addr.s_addr = INADDR_ANY; //   IP address of machine
    serv_addr.sin_port = htons(portno);     //   Port in network byte order

    if ( bind(                              // Bind socket to address/port
           sockfd,                          //   Socket
           (struct sockaddr *) &serv_addr,  //   Serve Address/Port
           sizeof(serv_addr)
         ) < 0) {
        perror("ERROR binding socket to address");
        exit(1);
    }

    // 3. Wait for client to connect
    listen(sockfd,5);
    struct sockaddr_in cli_addr;            // Clent internet address

    int clilen, newsockfd, n;
    for (;;) {
        clilen = sizeof(cli_addr);          // Size of address may change
        newsockfd = accept(                 // Wait for client connection
            sockfd,
            (struct sockaddr *) &cli_addr,
            &clilen);
        if (newsockfd < 0) {
            perror("ERROR on accept");
            continue;                       // Bad accept; skip the rest
        }

        // 4. Send Hello, World
        n = write(newsockfd,"Hello, World\n",13);
        if (n < 0)
            perror("ERROR writing to socket");

        close(newsockfd);                   // Cleanup
    }

    close(sockfd);
    return 0;
}

/* Test:
$ telnet localhost 9001
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
Hello, World
Connection closed by foreign host.

$ ftp localhost 9001
Connected to localhost.
Hello, World
ftp> bye
*/

/*
# This: server-loop.c
# Prev: server-send-hello-world.c
# Next: http-server-initial.c

# Build & run:
gcc server-loop.c -o server && ./server 9001
*/

