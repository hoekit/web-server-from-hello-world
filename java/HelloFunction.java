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
# This: HelloFunction.java
# Prev: HelloLiteralString.java
# Next: FetchFunctionHappy.java

# Build & run:
JNAME=HelloFunction; javac ${JNAME}.java && java ${JNAME}
*/

