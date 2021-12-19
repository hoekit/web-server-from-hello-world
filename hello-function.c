#include <stdio.h>

int hello(char* name) {
    printf("Hello, %s!\n", name);
    return 0;
}

int main() {
    char *name = "function";            // another way to define string literals
    return hello(name);                 // Hello function!
}

/*
# This: hello-function.c
# Prev: hello-literal-string.c
# Next: fetch-function-happy.c

# Build & run:
gcc hello-function.c -o hello && ./hello
*/
