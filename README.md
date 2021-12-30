# Web Server from "hello world"
A project to build a simple web server in C starting from [hello-world.c](txt/hello-world.c.txt).

**Read source code in the following sequence:**

- [hello-world.c](txt/hello-world.c.txt)
- [hello-literal-string.c](txt/hello-literal-string.c.txt)
- [hello-function.c](txt/hello-function.c.txt)
- [fetch-function-happy.c](txt/fetch-function-happy.c.txt)
- [fetch-function-error-handling.c](txt/fetch-function-error-handling.c.txt)
- [fetch-cli-error-handling.c](txt/fetch-cli-error-handling.c.txt)

**Learn more about:**

- [Sockets](learn-about-sockets.md)
- [CGI](learn-about-cgi.md)
- [Sending Images](learn-about-sending-images.md)
- [Implementation Notes](implementation-notes.md)
    - Description of various considerations when implementing the server

**Continue with client-server version of hello world:**

- [Client Server Overview](hello-client-server-overview.md)
- [server-initial.c](txt/server-initial.c.txt)
- [server-with-socket.c](txt/server-with-socket.c.txt)
- [server-bind-to-port.c](txt/server-bind-to-port.c.txt)
- [server-wait-for-connection.c](txt/server-wait-for-connection.c.txt)
- [server-send-hello-world.c](txt/server-send-hello-world.c.txt)
- [server-loop.c](txt/server-loop.c.txt)

**Start building the HTTP server:**

- [http-server-initial.c](txt/http-server-initial.c.txt)
- [http-server-fetch.c](txt/http-server-fetch.c.txt)
- [http-server-with-get.c](txt/http-server-with-get.c.txt)
- [http-server-404-on-not-found.c](txt/http-server-404-on-not-found.c.txt)
- [http-server-serve-image-jpeg.c](txt/http-server-serve-image-jpeg.c.txt)
- [http-server-list-dir.c](txt/http-server-list-dir.c.txt)
- [http-server-command-line-args.c](txt/http-server-command-line-args.c.txt)


**Obseleted**

- [hello-server.c](txt/hello-server.c.txt)
    - Same version in the Linux Howto [Sockets Tutorial](https://www.linuxhowtos.org/C_C++/socket.htm)
- [hello-client.c](txt/hello-client.c.txt)
    - Same version in the Linux Howto [Sockets Tutorial](https://www.linuxhowtos.org/C_C++/socket.htm)

**TODO**

- Create a simple PC-Android file transfer tool
    - See maint.mdown - Create a simple pc-android file transfer tool


**References**

- HTTP 1.0 (RFC 1945) / [1996](https://datatracker.ietf.org/doc/html/rfc1945)
- AltHttpd: Simple webserver in a single C file / [2001](https://sqlite.org/althttpd/doc/trunk/althttpd.md)
- John Marshall's HTTP made really easy / [2012](https://jmarshall.com/easy/http/)
- Linux Howto Sockets Tutorial / [web](https://www.linuxhowtos.org/C_C++/socket.htm)
- Beej's Guide to Network Programming / [2021](http://www.beej.us/guide/bgnet/)
- Hands-On Network Programming with C / [2019](https://github.com/codeplea/Hands-On-Network-Programming-with-C)
- Sockets and signals / [2019](https://opensource.com/article/19/4/interprocess-communication-linux-networking)
- GNU Libmicrohttpd / [2021](http://www.gnu.org/software/libmicrohttpd/)

