import java.net.ServerSocket;
import java.net.Socket;
import java.io.DataOutputStream;
import java.io.IOException;

public class ServerLoop {

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
            for (;;) {
                // 3. Wait for client to connect
                step = "accept connection";
                socket = server.accept();

                // 4. Setup output socket
                step = "get socket for writing";
                sOut = new DataOutputStream(socket.getOutputStream());

                // 4.1 Send Hello, Loop!
                step = "write to socket";
                sOut.writeUTF("Hello, Loop!\n");

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

/* Test:
telnet localhost 5001
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
Hello, World!
Connection closed by foreign host.
*/

/*
# This: ServerLoop.java
# Prev: ServerSendHelloWorld.java
# Next: -

# Build & run:
JNAME=ServerLoop; javac ${JNAME}.java && java ${JNAME}
*/

