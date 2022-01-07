import java.net.ServerSocket;
import java.net.Socket;
import java.io.DataOutputStream;
import java.io.IOException;

public class HttpServerInitial {

    // Print error step and exit
    private static void Stop(String step, Exception e) {
        System.out.println(
            "ERROR " + step + ": " + e.getMessage());
        System.exit(1);
    }

    public static void main(String[] args) {
        ServerSocket server = null;
        Socket socket = null;
        DataOutputStream sOut = null;
        String step = "";

        try {
            // 1. Create a socket
            // 2. Bind socket to a port
            step = "create server on port 5001";
            server = new ServerSocket(5001);

            System.out.println("Wait for connection. Ctrl-C to kill.");
            int n = 0;
            for (;;) {
                // 3. Wait for client to connect
                step = "accept connection";
                socket = server.accept();

                // 4. Setup output socket
                step = "get socket for writing";
                sOut = new DataOutputStream(socket.getOutputStream());

                // 4.1 Send Hello, World! (N)
                step = "write to socket";
                sOut.writeUTF(
                      "HTTP/1.0 200 OK\n"
                    + "Content-Type: text/html\n"
                    + "\n"
                    + "<html><body><h2>Hello, World! ("
                      + (n++)
                      + ")</h2></body></html>\n"
                );

                // 4.2 Cleanup
                step = "close socket reader";
                sOut.close();
                step = "close socket";
                socket.close();
            }
        }catch( IOException e ){
            Stop(step, e);
        }

        try {
            step = "close listener";
            server.close();
        }catch( IOException e ){
            Stop(step, e);
        }

    }
}

/* Return HTTP/1.0 response
  - This version sends a simple HTTP/1.0 response when the client
    connects at http://127.0/0.1:5001
*/

/*
# This: HttpServerInitial.java
# Prev: ServerLoop.java
# Next: -

# Build & run:
JNAME=HttpServerInitial; javac ${JNAME}.java && java ${JNAME}
*/

