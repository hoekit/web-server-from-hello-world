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
#include <dirent.h>
    // DIR *opendir(const char *name);
    // struct dirent *readdir(DIR *dirp);
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

static char *zRoot = ".";                   // Default document root
static int zPort = 9001;                    // Default server port

static int swrite_file(int sockfd, char* fname) {
    // Helper to write content of a file to the socket

    int fd = open(fname, O_RDONLY);
    if (fd == -1) {
        dprintf(sockfd,"404 Not Found\n");
        return 1;
    }

    // read from fname and write to socket
    char buf[512];
    int n;
    for (;;) {
        n = read(fd,buf,sizeof(buf));

        if (n > 0) {
            if (write(sockfd,buf,n)<0) {            // write contents to socket
                perror("ERROR writing socket");
            }
        } else if (n == 0) {                        // EOF
            close(fd);
            return 0;
        } else {                                    // error on read
            perror("ERROR reading file");
            close(fd);
            return 1;
        }
    }
    return 0;
}
static int canRead(char *res) {
    // Return 1 if can open for reading, 0 otherwise
    FILE *fp = fopen(res, "r");
    if (!fp)
        return 0;                           // Cannot open for reading
    fclose(fp);                             // close file
    return 1;
}
static void getExt(const char *path, char *ext) {
    int szExt = sizeof(ext);
    bzero(ext,szExt);
    char *dot = strrchr(path, '.');         // find last '.' char
    if (dot != NULL) {                      // if found
        strncpy(ext,++dot,szExt-1);         //   copy after '.' to ext
    }
}
const char *lookupMime(const char *ext) {
    struct {
        const char *ext;
        const char *mime;
    } aMime[] = {
        { "c",      "text/plain; charset=utf-8" },
        { "css",    "text/css; charset=utf-8" },
        { "html",   "text/html; charset=utf-8" },
        { "ico",    "image/x-icon" },
        { "jpg",    "image/jpeg" },
        { "md",     "text/plain; charset=utf-8" },
        { "mdown",  "text/plain; charset=utf-8" },
        { "png",    "image/png" },
        { "txt",    "text/plain; charset=utf-8" },
        { NULL,     NULL },                         // End-of-array marker
    };
    // Linear search
    for( int i=0; aMime[i].ext!=NULL; i++ ){
        if( strcmp(ext,aMime[i].ext)==0 ){
            return aMime[i].mime;
        }
    }
    return "application/octet-stream";
}

/*
** Check if pathname is a directory
*/
static int isDir (const char *pathname) {
    int len = strlen(pathname);
    if (pathname[len-1] == '/') {
        return 1;
    } else {
        return 0;
    }
}

/*
** Generate directory HTML page and return the size of the generated page
*/
static int SendDirPage(int fd, const char *path, const char *base) {
    dprintf(fd,"HTTP/1.0 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "\r\n"
        "<html>\n"
        "<head><meta charset=\"utf-8\"/></head>\n"
        "<body>\n"
        "<h2>Directory %s</h2>\n<pre><ul>\n",base);

    // Iterate over directory entries
    DIR *dirp = opendir(path);
    struct dirent *dp;
    if (dirp == NULL) {
        perror("ERROR opening directory");
        return -1;
    }
    for (;;) {
        dp = readdir(dirp);
        if (dp == NULL)
            break;

        if ( strcmp(dp->d_name,".") == 0 || strcmp(dp->d_name, "..") == 0 )
            continue;

        if (dp->d_type == 4) {                  // Directory
            // printf("%s/\n", dp->d_name);
            dprintf(fd,"  <li><a href=\"%s/%s/\">%s/</a>\n",
                base,dp->d_name,dp->d_name);
        } else if (dp->d_type == 8) {           // Files
            // printf("%s\n", dp->d_name);
            dprintf(fd,"  <li><a href=\"%s\">%s</a>\n",dp->d_name,dp->d_name);
        } else {                                // Symlinks
            // printf("%d %s\n", dp->d_type, dp->d_name);
            dprintf(fd,"  <li>%s\n",dp->d_name);
        }
    }

    dprintf(fd,"</ul></code>\n</body>\n</html>");
}

/*
** Tell client no such resource
*/
static void NotFound(int fd) {
    dprintf(fd,
        "HTTP/1.0 404 Not Found\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "404 Not Found\n");
}

int main(int argc, char *argv[]) {

    // Process command-line arguments
    while( argc>1 && argv[1][0]=='-' ){
        char *zOpt = argv[1];
        char *zVal = argc>2 ? argv[2] : NULL;

        if( strcmp(zOpt,"--root")==0 ){
            zRoot = zVal==NULL ? zRoot : zVal;
            printf("zRoot: %s\n",zRoot);
        }else if( strcmp(zOpt,"--port")==0 ){
            zPort = zVal==NULL ? zPort : atoi(zVal);
            printf("zPort: %d\n",zPort);
        }

        argc -= 2;
        argv += 2;
    }

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
    struct sockaddr_in serv_addr;           // Internet address with port
    bzero( (char*) &serv_addr,              //   Initialize struct to zeros '\0'
           sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;         //   IPv4 Internet protocols
    serv_addr.sin_addr.s_addr = INADDR_ANY; //   IP address of machine
    serv_addr.sin_port = htons(zPort);      //   Port in network byte order

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

    int clilen, nIn;
    int newsockfd = -1;                     // Fake, invalid socket
    char buf[5120], req[5120];
    for (;;) {
        bzero(buf,5120);                    // Overwrite buffer with zeros
        bzero(req,5120);                    // Overwrite req with zeros
        clilen = sizeof(cli_addr);          // Size of address may change
        if( newsockfd>-1 )                  // Close valid open sockets
            close(newsockfd);               //   from previous connection
        newsockfd = accept(                 // Wait for client connection
            sockfd,
            (struct sockaddr *) &cli_addr,
            &clilen);
        if (newsockfd < 0) {
            perror("ERROR on accept");
            continue;                       // Bad accept; skip the rest
        }

        // Read client request line
        nIn = read(newsockfd,buf,5119);     // Read request line

        if (nIn < 0) {                      // On read socket error
            perror("ERROR reading socket");
            continue;
        } else if (nIn == 0) {              // Zero bytes read
            continue;
        }
        // printf("\nRequest:\n%s", buf);

        // Read up (but excluding) the first newline
        for( int i=0; i<strlen(buf) && buf[i]!='\n' && buf[i]!='\r'; i++ ){
            req[i]=buf[i];
        }
        printf("%s\n", req);

        // Split line into method, path and version
        char method[16], rpath[256], version[16];
        bzero(method,16);
        bzero(rpath,256);
        bzero(version,16);
        char *target[3] = {                 // Array of pointers to
            method, rpath, version          //   each target
        };
        int t = 0;                          // Start with method as target
        int j = 0;                          // Index into each target
        for( int i=0; i<strlen(req); i++ ){
            if (req[i] == ' ') {            // When space is seen
                t++;                        // Switch to new target
                j = 0;                      // Reset target index
            } else {
                target[t][j++] = req[i];    // Store char into target
            }
        }
        // printf("Method: %s Path: %s Version: %s",
        //        method, rpath, version);

        // Guard: Path must start with /
        if( rpath[0]!='/' ) {
            NotFound(newsockfd);
            continue;
        }

        // Prefix request path with www
        char path[256] = "";
        strcat(path, zRoot);                // Prefix with document root
        strcat(path, rpath);
        // printf("Path: %s\n", path);

        // Handle missing resources
        if (canRead(path) != 1) {
            NotFound(newsockfd);
            continue;                       // Skip further processing
        }

        // Split rpath into base and file
        char *z;
        char *base = rpath;
        char *file;
        for( z=rpath; *z; z++ ){
            if( *z=='/' )
                file = z;
        }
        *file = 0;
        file++;
        // printf("Base: %s\n", base);
        // printf("File: %s\n", file);

        // Extract extension from path
        char ext[10];
        getExt(file,ext);
        // printf("Extension: '%s'\n", ext);

        // Determine MIME type from extension
        const char *mime = lookupMime(ext);
        // printf("MIME Type: '%s'\n", mime);

        // Determine size of content in bytes
        struct stat sbuf;
        if (stat(path,&sbuf) == -1) {
            perror("ERROR Cannot stat");
        }
        long int fsize = sbuf.st_size;
        // printf("Size of file: %ld\n",fsize);

        if (isDir(path)) {
            SendDirPage(newsockfd,path,base);
            // printf("Page: \n%s",page);
        } else {
            // Send the file specified in the path
            dprintf(newsockfd,"HTTP/1.0 200 OK\r\n"
                "Content-Type: %s\r\n"
                "Content-Length: %ld\r\n"
                "\r\n",mime,fsize);
            swrite_file(newsockfd,path);
        }

        usleep(1000);
    }

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
  - List directories
  - Handles command-line arguments:
    --root DOCUMENT_ROOT
    --port SERVER_PORT

- This version cannot:
  - Handle HTTP/1.1 yet
  - Handle client request > 512 bytes
  - Handle requests for generic images
  - Handle URIs with query parameters
*/
/*
# This: http-server-command-line-args.c
# Prev: http-server-list-dir.c
# Next: -

# Build & run:
gcc http-server-command-line-args.c -o http && ./http
*/

