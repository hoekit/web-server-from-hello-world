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
#include <sys/stat.h>
    // int stat(const char *pathname, struct stat *statbuf);
#include <fcntl.h>
    // int open(const char *pathname, int flags);
#include <strings.h>
    // void bzero(void *s, size_t n);
#include <string.h>
    // size_t strlen(const char *s);
    // char *strrchr(const char *s, int c);
    // char *strncpy(char *dest, const char *src, size_t n);
    // int strcmp(const char *s1, const char *s2);
    // char *strstr(const char *haystack, const char *needle);
#include <netinet/in.h>
    // struct sockaddr_in
#include <arpa/inet.h>
    // uint16_t htons(uint16_t hostshort);
#include <unistd.h>
    // ssize_t write(int fd, const void *buf, size_t count);

static int swrite(int sockfd, const char *str) {
    // Helper to write a string to the socket
    if (write(sockfd, str, strlen(str)) < 0) {
        perror("ERROR writing to socket");
    }
}
static int swrite_file(int sockfd, char* fname) {
    // Helper to write content of a file to the socket

    // open fname
    int fd = open(fname, O_RDONLY);
    if (fd == -1) {
        dprintf(sockfd,"404 Not Found\n");  // print content
        return 1;
    }

    // read from fname and write to socket
    char con[512];                          // store content in a char array
    int n;
    for (;;) {
        n = read(fd,con,sizeof(con));

        if (n < 0) {                        // error on read
            perror("ERROR reading file");
            close(fd);
            return 1;
        }
        if (n == 0) {                       // EOF
            close(fd);
            return 0;
        }
        if (n > 0) {
            if (write(sockfd,con,n)<0) {            // write contents to socket
                perror("ERROR writing socket");
            }
        }
    }
    return 0;
}
static int res_not_found(char *res) {
    // Check if resource can be accessed
    // If cannot read, return 1
    // If can be read, return 0

    FILE *fp = fopen(res, "r");
    if (!fp)
        return 1;

    fclose(fp);                             // close file
    return 0;
}
static void get_extension(const char *path, char *ext) {
    bzero(ext,sizeof(ext));
    char *dot = strrchr(path, '.');         // find last '.' char
    if (dot != NULL) {                      // if found
        strncpy(ext,++dot,9);               //   copy after '.' to ext
    }
}
static void get_mime_type(const char *ext, char *mime, int mime_len) {
    char *ext_lookup[8] = {
        "css",
        "html",
        "ico",
        "jpg",
        "md",
        "mdown",
        "png",
        "txt",
    };
    char *mime_lookup[8] = {
        "text/css; charset=utf-8",
        "text/html; charset=utf-8",
        "image/x-icon",
        "image/jpeg",
        "text/plain; charset=utf-8",        // .md
        "text/plain; charset=utf-8",        // .mdown
        "image/png",
        "text/plain; charset=utf-8",        // .txt
    };
    for (int i=0; i<8; i++) {
        if (strcmp(ext,ext_lookup[i]) == 0) {
            // printf("mime_lookup[%d]: %s\n",i,mime_lookup[i]);
            strncpy(mime,mime_lookup[i],mime_len-1);
            break;
        }
    }
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

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
            &(int){1}, sizeof(int)) < 0)
        perror("setsockopt(SO_REUSEADDR) failed");

    // 2. Bind socket to a port
    int portno;
    if (argc < 2) {
        portno = 9001;                      // Default port
    } else {
        portno = atoi(argv[1]);             // Get port number from argument
    }

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
    char buffer[5120], req[5120];
    for (;;) {
        bzero(buffer,5120);                 // Overwrite buffer with zeros
        bzero(req,5120);                    // Overwrite req with zeros
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
        n = read(newsockfd,buffer,5119);    // Read request line

        if (n < 0) {                        // On read socket error
            perror("ERROR reading socket");
            continue;
        } else if (n == 0) {                // Zero bytes read
            // printf("INFO zero bytes read\n");
            close(newsockfd);
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
        char path2[256] = ".";
        strcat(path2, path);
        // printf("Path: %s\n", path2);

        // Handle missing resources
        if (res_not_found(path2) == 1) {
            dprintf(newsockfd,"HTTP/1.0 404 Not Found\n");
            dprintf(newsockfd,"Content-Type: text/plain\n");
            dprintf(newsockfd,"\n");
            dprintf(newsockfd,"404 Not Found\n");
            close(newsockfd);               // Cleanup
            continue;                       // Skip further processing
        }

        // Extract extension from path
        char ext[10];
        get_extension(path2,ext);
        // printf("Extension: '%s'\n", ext);

        // Determine MIME type from path
        char mime[256] = "application/octet-stream";
        get_mime_type(ext,mime,sizeof(mime));
        // printf("MIME Type: '%s'\n", mime);

        // Determine size of content in bytes
        struct stat sbuf;
        if (stat(path2,&sbuf) == -1) {
            perror("ERROR Cannot stat");
        }
        long int fsize = sbuf.st_size;
        // printf("Size of file: %ld\n",fsize);

        // Send the file specified in the path
        char str[512];
        dprintf(newsockfd,"HTTP/1.0 200 OK\n");
        dprintf(newsockfd,"Content-Type: %s\n",mime);
        dprintf(newsockfd,"Content-Length: %ld\n",fsize);
        dprintf(newsockfd,"\n");

        if (strstr(mime,"text") != NULL) {
            swrite_file(newsockfd,path2);
        } else {
            swrite_file(newsockfd,path2);
        }

        // printf("Close newsockfd\n");
        usleep(1000);
        close(newsockfd);                   // Cleanup
    }

    printf("Close sockfd");
    close(sockfd);
    return 0;
}

/*
# Send HTTP/1.0 response from html file

- This version can:
  - handle a HTTP/1.0 GET request
    e.g. GET /www/hello.html HTTP/1.1
  - Send 404 Not Found if the resource is missing
  - Send .jpg resources

- This version cannot:
  - Handle directories
  - Handle HTTP/1.1 yet
  - Handle client request > 512 bytes
  - Handle requests for generic images
  - Handle URIs with query parameters
*/
/* Issues (0)
*/
/* Fixed Problems (2)
**
** -------------------------------------------------------------------------
** Problem: Moving back and forth in browser history cause server to exit
**
** Steps to replicate:
**   1. Start http server
**   2. View the following in sequence
**   - GET /README.md
**   - GET /learn-about-sending-images.md
**   - GET /README.md
**   - GET /learn-about-sockets.md
**   - GET /README.md
**   - GET /learn-about-sockets.md
**   - GET /README.md
**   - GET /learn-about-cgi.md
**   - GET /README.md
**   - GET /learn-about-sending-images.md
**   - GET /README.md
**   - GET /txt/hello-server.c.txt
**   3. Move back and forth
**   4. At some point the server will exit
**
** Solution:
**
** The error is caused by an unhandled condition where the client makes
** a connection but sends zero bytes.
**
** To fix this, check for this condition and when found, close the socket
** and wait again for another connection.
**
** -------------------------------------------------------------------------
** Problem: Consistently receive 'The connection was reset' error
**
** Steps to replicate:
**   1. Start http server
**   2. Get /README.md
**   3. Get /learn-about-sending-images.md
**
** Solution: Increase request buffer
**
**   Increasing the request buffer from 512 to 5120 seems to have fixed
**   the problem.
*/
/*
# This: http-server-serve-image-jpeg.c
# Prev: http-server-404-on-not-found.c
# Next: -

# Build & run:
gcc http-server-serve-image-jpeg.c -o http && ./http 9001
*/

