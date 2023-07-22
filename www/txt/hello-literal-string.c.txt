#include <stdio.h>

int main() {
    char name[60] = "literal string";       // string literals in double quotes
    printf("Hello, %s!\n", name);
    return 0;
}

/*
# This: hello-literal-string.c
# Prev: hello-world.c
# Next: hello-function.c

# Build & run:
gcc hello-literal-string.c -o hello && ./hello
*/
