/* A simple server in the internet domain using TCP
   The port number is passed as an argument
   Modified from Linux Howto Sockets Tutorial
   - https://www.linuxhowtos.org/C_C++/socket.htm

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
            &(int){1}, sizeof(int)) < 0)
        error("setsockopt(SO_REUSEADDR) failed");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
            sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,
        &clilen);
    if (newsockfd < 0) error("ERROR on accept");
    n = write(newsockfd,"Hello, World\n",13);
    if (n < 0) error("ERROR writing to socket");
    close(newsockfd);
    close(sockfd);
    return 0; 
}

/* Problems & Solutions

----
Problem: Restarting the server at the same port has errors

    $ ./server 9001
    ERROR on binding: Address already in use

Cause: Client closed socket after server

    The server has closed the socket but before the client did so it is
    in a TIME_WAIT state.

Solution: Use SO_REUSEADDR

    This solution allows bind to use the socket if it is in a TIME_WAIT state.
    See SO_REUSEADDR in:
    - https://www.man7.org/linux/man-pages/man7/socket.7.html
    See setsockopt() in:
    - https://www.man7.org/linux/man-pages/man2/setsockopt.2.html

    Use SO_REUSEADDR like so:

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
        error("setsockopt(SO_REUSEADDR) failed");
*/

/*
# This: hello-server.c
# Prev: learn-about-cgi.md
# Next: hello-client.c

# Build & run:
gcc hello-server.c -o server && ./server 9001
*/

