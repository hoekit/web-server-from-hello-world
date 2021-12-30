import java.io.*;
// FileInputStream
// FileNotFoundException
// IOException

public class FetchFunctionErrorHandling {

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
        String fname = "no-such-file.java";
        fetch(fname);
    }
}

/*
# This: FetchFunctionErrorHandling.java
# Prev: FetchFunctionHappy.java
# Next: -

# Build & run:
JNAME=FetchFunctionErrorHandling; javac ${JNAME}.java && java ${JNAME}
*/

