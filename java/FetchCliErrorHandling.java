import java.io.*;
// FileInputStream
// FileNotFoundException
// IOException

public class FetchCliErrorHandling {

    private static void fetch(String fname) {
        try{
            FileInputStream fis = new FileInputStream(fname);
            int i;
            byte buf[] = new byte[100];
            while( (i = fis.read(buf))>0 ){
                String str = new String(buf,0,i);
                System.out.print(str);
            }
            fis.close();
        }catch( FileNotFoundException e ){
            System.out.println("404 Not Found: "+fname);
        }catch( IOException e ){
            System.out.println("500 Server Error: Read failed");
        }
    }

    public static void main(String[] args) {
        if( args.length>0 ){
            fetch(args[0]);
        } else {
            System.out.println(
                "\n  USAGE: java FetchCliErrorHandling FILE\n");
        }
    }
}

/*
# This: FetchCliErrorHandling.java
# Prev: FetchFunctionErrorHandling.java
# Next: ServerInitial.java

# Build & run:
JNAME=FetchCliErrorHandling; javac ${JNAME}.java && java ${JNAME}
*/

