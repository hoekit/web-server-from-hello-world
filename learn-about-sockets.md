<small>[Home](README.md) / Sockets</small>

----
# Learn about sockets

Read about it [here](https://www.linuxhowtos.org/C_C++/socket.htm). It's a Linux Howto so may not apply to other OSes.

**Sockets**

Sockets connect clients and servers.
A socket is one end of an interprocess communication (IPC) channel.

**Establishing client-side sockets**

1. Create a socket with the [socket()](https://www.linuxhowtos.org/manpages/2/socket.htm) system call.
2. Connect to server using the [connect()](https://www.linuxhowtos.org/manpages/2/connect.htm) system call.
3. Send and receive data using [read()](https://www.linuxhowtos.org/manpages/2/read.htm) and [write()](https://www.linuxhowtos.org/manpages/2/write.htm) system calls.

**Establishing server-side sockets**

1. Create a socket with the [socket()](https://www.linuxhowtos.org/manpages/2/socket.htm) system call.
2. Bind socket to a port number on the host using the [bind()](https://www.linuxhowtos.org/manpages/2/bind.htm) system call.
3. List for connections with the [listen()](https://www.linuxhowtos.org/manpages/2/listen.htm) system call.
4. Accept connections with the [accept()](https://www.linuxhowtos.org/manpages/2/accept.htm) system call.
5. Send and receive data.

**Download server.c and client.c**

Do the following in the project folder:

    mkdir -p/learn
    cd learn
    wget https://www.linuxhowtos.org/data/6/server.c    # 55 lines
    wget https://www.linuxhowtos.org/data/6/client.c    # 57 lines

Build the server and client:

    gcc server.c -o server
    gcc client.c -o client

Run the server and client:

    # On one terminal
    ./server 9000

    # On another terminal
    ./client localhost 9000

----
**File: server.c**

    /* A simple server in the internet domain using TCP
       The port number is passed as an argument */
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
         char buffer[256];
         struct sockaddr_in serv_addr, cli_addr;
         int n;
         if (argc < 2) {
             fprintf(stderr,"ERROR, no port provided\n");
             exit(1);
         }
         sockfd = socket(AF_INET, SOCK_STREAM, 0);
         if (sockfd < 0) 
            error("ERROR opening socket");
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
         newsockfd = accept(sockfd, 
                     (struct sockaddr *) &cli_addr, 
                     &clilen);
         if (newsockfd < 0) 
              error("ERROR on accept");
         bzero(buffer,256);
         n = read(newsockfd,buffer,255);
         if (n < 0) error("ERROR reading from socket");
         printf("Here is the message: %s\n",buffer);
         n = write(newsockfd,"I got your message",18);
         if (n < 0) error("ERROR writing to socket");
         close(newsockfd);
         close(sockfd);
         return 0; 
    }

----
**File: client.c**

    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <string.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netdb.h> 

    void error(const char *msg)
    {
        perror(msg);
        exit(0);
    }

    int main(int argc, char *argv[])
    {
        int sockfd, portno, n;
        struct sockaddr_in serv_addr;
        struct hostent *server;

        char buffer[256];
        if (argc < 3) {
           fprintf(stderr,"usage %s hostname port\n", argv[0]);
           exit(0);
        }
        portno = atoi(argv[2]);
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) 
            error("ERROR opening socket");
        server = gethostbyname(argv[1]);
        if (server == NULL) {
            fprintf(stderr,"ERROR, no such host\n");
            exit(0);
        }
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr, 
             (char *)&serv_addr.sin_addr.s_addr,
             server->h_length);
        serv_addr.sin_port = htons(portno);
        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
            error("ERROR connecting");
        printf("Please enter the message: ");
        bzero(buffer,256);
        fgets(buffer,255,stdin);
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0) 
             error("ERROR writing to socket");
        bzero(buffer,256);
        n = read(sockfd,buffer,255);
        if (n < 0) 
             error("ERROR reading from socket");
        printf("%s\n",buffer);
        close(sockfd);
        return 0;
    }

----

    /*
    # This: learn-about-sockets.md
    # Prev: fetch-cli-error-handling.c
    # Next: learn-about-cgi.md
    */

