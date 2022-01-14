import java.net.ServerSocket;
import java.net.Socket;
import java.io.DataOutputStream;
import java.io.DataInputStream;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.io.FileInputStream;

public class HttpServer404 {

    // Print error step and exit
    private static void Stop(String step, Exception e) {
        System.out.println(
            "ERROR " + step + ": " + e.getMessage());
        System.exit(1);
    }

    private static boolean ResFound(String fname) {
        try{
            FileInputStream sIn = new FileInputStream(fname);
            sIn.close();
            return true;
        }catch( FileNotFoundException e ){
            System.out.println("404 Not Found: "+fname);
        }catch( IOException e ){
            System.out.println("500 Server Error: failed close file: "+fname);
        }
        return false;
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
        DataInputStream sIn = null;
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
                if( socket!=null ){
                    step = "close previously opened socket, if any";
                    socket.close();
                }
                // 3. Wait for client to connect
                step = "accept connection";
                socket = server.accept();

                // Read request
                if( sIn!=null ){
                    step = "close previously opened socket reader, if any";
                    sIn.close();
                }
                sIn = new DataInputStream(socket.getInputStream());
                // Assume max request 5120 bytes (FIXME)
                byte[] buf = new byte[5120];
                sIn.read(buf);

                String req = new String(buf);
                // reqPart: METHOD URL PROTOCOL
                String[] reqPart = req.split(" ",3);
                if( reqPart.length<3 )                  // All parts defined
                    continue;

                // 4. Setup output socket
                if( sOut!=null ){
                    step = "close previously opened socket writer, if any";
                    sOut.close();
                }
                step = "get socket for writing";
                sOut = new DataOutputStream(socket.getOutputStream());

                // Check whether URL resource exists
                if( !ResFound("www"+reqPart[1]) ){
                    sOut.writeUTF(
                          "HTTP/1.0 404 OK\r\n"
                        + "Content-Type: text/plain\r\n"
                        + "\r\n"
                        + "404 Not Found\n"
                    );
                    continue;
                }

                // 4.1 Send Hello, World! (N)
                step = "write to socket";
                sOut.writeUTF(
                      "HTTP/1.0 200 OK\n"
                    + "Content-Type: text/html\n"
                    + "\n"
                );
                // Assumes METHOD = GET (FIXME)
                // Assumes valid PROTOCOL (FIXME)
                sWriteFile(sOut,"www"+reqPart[1]);
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

- This version can:
  - handle a HTTP/1.0 GET request e.g. GET /hello.html HTTP/1.0
  - send 404 Not Found if requested resource is missing
*/

/*
# This: HttpServer404.java
# Prev: HttpServerWithGet.java
# Next: -

# Build & run:
JNAME=HttpServer404; javac ${JNAME}.java && java ${JNAME}
*/

