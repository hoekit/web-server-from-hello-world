public class HelloFunction {
    private static void hello(String name) {
        // Use println is more idiomatic
        System.out.println("Hello, " + name);
    }

    public static void main(String[] args) {
        String name = "function";
        hello(name);
    }
}

/*
# This: HelloLiteralString.java
# Prev: HelloWorld.java
# Next: HelloFunction.java

# Build & run:
JNAME=HelloFunction; javac ${JNAME}.java && java ${JNAME}
*/

