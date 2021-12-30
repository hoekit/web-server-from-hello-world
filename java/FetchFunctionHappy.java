import java.io.FileInputStream;

public class FetchFunctionHappy {

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
        }catch( Exception e ){
            System.out.println(e);
        }
    }

    public static void main(String[] args) {
        String fname = "HelloFunction.java";
        fetch(fname);
    }
}

/*
# This: FetchFunctionHappy.java
# Prev: HelloFunction.java
# Next: -

# Build & run:
JNAME=FetchFunctionHappy; javac ${JNAME}.java && java ${JNAME}
*/

