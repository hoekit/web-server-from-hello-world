import java.net.ServerSocket;
import java.net.Socket;
import java.io.IOException;

public class ServerWaitForConnection {
    public static void main(String[] args) {
        // 1. Create a socket
        // 2. Bind socket to a port
        ServerSocket server = null;
        try {
            server = new ServerSocket(5001);
        }catch( IOException e ){
            System.out.println(
                "ERROR create server on port 5001: "
                + e.getMessage());
            System.exit(1);
        }

        // 3. Wait for client to connect
        System.out.println("Server: Wait for Connection"
            + " - Ctrl-C to kill.");
        Socket socket = null;
        try {
            socket = server.accept();
        }catch( IOException e ){
            System.out.println(
                "ERROR accept connection: " + e.getMessage());
            System.exit(1);
        }

        // 4. Send Hello, World
    }
}

/*
# This: ServerWaitForConnection.java
# Prev: ServerWithSocket.java
# Next: ServerSendHelloWorld.java

# Build & run:
JNAME=ServerWaitForConnection; javac ${JNAME}.java && java ${JNAME}
*/

