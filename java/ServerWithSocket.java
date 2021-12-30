import java.net.ServerSocket;
import java.io.IOException;

public class ServerWithSocket {
    public static void main(String[] args) {
        // 1. Create a socket
        // 2. Bind socket to a port
        try {
            ServerSocket server = new ServerSocket(5001);
        }catch( IOException e ){
            System.out.println(e.getMessage());
        }

        // 3. Wait for client to connect
        // 4. Send Hello, World
        System.out.println("Server: With socket");
    }
}

/*
# This: ServerWithSocket.java
# Prev: ServerInitial.java
# Next: ServerWaitForConnection.java

# Build & run:
JNAME=ServerWithSocket; javac ${JNAME}.java && java ${JNAME}
*/

