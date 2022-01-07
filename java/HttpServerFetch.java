import java.net.ServerSocket;
import java.net.Socket;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.io.FileInputStream;

public class HttpServerFetch {

    // Print error step and exit
    private static void Stop(String step, Exception e) {
        System.out.println(
            "ERROR " + step + ": " + e.getMessage());
        System.exit(1);
    }

    private static void sWriteFile(DataOutputStream sOut, String fname) {
        try{
            FileInputStream sIn = new FileInputStream(fname);
            int i;
            byte buf[] = new byte[100];
            while( (i = sIn.read(buf))>0 ){
                sOut.write(buf,0,i);
            }
            sIn.close();
        }catch( FileNotFoundException e ){
            System.out.println("404 Not Found: "+fname);
        }catch( IOException e ){
            System.out.println("500 Server Error: Read failed");
        }
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
                );
                sWriteFile(sOut,"www/hello.html");

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

/* Return HTTP/1.0 response from HTML file
  - This version reads from a HTML file containing "Hello World!" and sends
    that as response when the client connects at http://127.0/0.1:5001
*/

/*
# This: HttpServerFetch.java
# Prev: HttpServerInitial.java
# Next: -

# Build & run:
JNAME=HttpServerFetch; javac ${JNAME}.java && java ${JNAME}
*/

