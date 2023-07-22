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
int main(int argc, char *argv[]) {
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
    char buffer[512], req[512];
    for (;;) {
        bzero(buffer,512);                  // Overwrite buffer with zeros
        bzero(req,512);                     // Overwrite req with zeros
        clilen = sizeof(cli_addr);          // Size of address may change
        newsockfd = accept(                 // Wait for client connection
            sockfd,
            (struct sockaddr *) &cli_addr,
            &clilen);
        if (newsockfd < 0) {
            perror("ERROR on accept");
            continue;                       // Bad accept; skip the rest
        }

        // Read client request line
        n = read(newsockfd,buffer,511);     // Read request line
        if (n < 0) {                        // On read socket error
            perror("ERROR reading socket");
            continue;
        }
        // printf("\nRequest:\n%s", buffer);

        // Get the first line
        int i = 0;
        while (i < strlen(buffer)) {
            req[i] = buffer[i];
            if (req[i] == '\n')
                break;
            i++;
        }
        // printf("\nRequest[0]:\n%s", req);
        printf("%s", req);

        // Split line into method, path and version
        char method[16], path[256], version[16];
        bzero(method,16);
        bzero(path,256);
        bzero(version,16);
        char *target[3] = {                 // Array of pointers to
            method, path, version           //   each target
        };
        int t = 0;                          // Start with method as target
        int j = 0;                          // Index into each target
        for (i = 0; i < strlen(req); i++) {
            if (req[i] == ' ') {            // When space is seen
                t++;                        // Switch to new target
                j = 0;                      // Reset target index
            } else {
                target[t][j++] = req[i];    // Store char into target
            }
        }
        // printf("Method: %s Path: %s Version: %s",
        //        method, path, version);

        // Prefix request path with www
        char path2[256] = "www";
        strcat(path2, path);
        // printf("Path: %s\n", path2);

        // Send the file specified in the path
        swrite(newsockfd,"HTTP/1.0 200 OK\n");
        swrite(newsockfd,"Content-Type: text/html\n");
        swrite(newsockfd,"\n");
        swrite_file(newsockfd,path2);

        close(newsockfd);                   // Cleanup
    }

    close(sockfd);
    return 0;
}

/*
# Send HTTP/1.0 response from html file

- This version can handle a HTTP/1.0 GET request
  e.g. GET /www/hello.html HTTP/1.1
- It cannot handle HTTP/1.1 yet
- It can only read 255 bytes of the client request
- It can only send text/html responses
- It can only send 200 OK responses even when the file is missing
- It cannot handle images

*/
/*
# This: http-server-with-get.c
# Prev: http-server-fetch.c
# Next: http-server-404-on-not-found.c

# Build & run:
gcc http-server-with-get.c -o http && ./http 9001
*/

