#include <stdio.h>
    // void perror(const char *s);
    // char *fgets(char *s, int size, FILE *stream);
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
#include <string.h>
    // size_t strlen(const char *s);
#include <netinet/in.h>
    // struct sockaddr_in
#include <arpa/inet.h>
    // uint16_t htons(uint16_t hostshort);
#include <unistd.h>
    // ssize_t write(int fd, const void *buf, size_t count);


int swrite(int sockfd, const char *str) {
    // Helper to write a string to the socket
    if (write(sockfd, str, strlen(str)) < 0) {
        perror("ERROR writing to socket");
    }
}
int swrite_file(int sockfd, char* fname) {
    // Helper to write content of a file to the socket

    // open fname
    FILE *fp = fopen(fname, "r");
    if (!fp) {
        swrite(sockfd,"404 Not Found\n");   // print content
        return 1;
    }

    // read from fname and write to socket
    char con[100];                          // store content in a char array
    while (fgets(con, 100, fp) != NULL)     // read from file into array
        swrite(sockfd,con);                 // write contents to socket

    fclose(fp);                             // close file
    return 0;
}

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
        swrite(newsockfd,"HTTP/1.0 200 OK\n");
        swrite(newsockfd,"Content-Type: text/html\n");
        swrite(newsockfd,"\n");
        swrite_file(newsockfd,"www/hello.html");

        close(newsockfd);                   // Cleanup
    }

    close(sockfd);
    return 0;
}

/* Send HTTP/1.0 response from html file

- This version reads from a html file containing hello world and sends
  that as the response when the client connects.
- The file is hard-coded to: www/hello.html

*/

/*
# This: http-server-fetch.c
# Prev: http-server-initial.c
# Next: http-server-with-get.c

# Build & run:
gcc http-server-fetch.c -o http && ./http 9001
*/

