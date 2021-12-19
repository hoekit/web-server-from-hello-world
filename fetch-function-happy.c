#include <stdio.h>
// Given an argument which is a file, fetch it, read it and print it

int fetch(char* fname) {
    // open fname
    FILE *fp = fopen(fname, "r");
    if (!fp) return 1;

    // read from fname and print
    char con[100];                          // store content in a char array
    while (fgets(con, 100, fp) != NULL)     // read from file into array
        printf("%s", con);                  // print content

    // close fname
    fclose(fp);
    return 0;
}

int main() {
    char *fname = "hello-function.c";
    return fetch(fname);
}

/*
# This: fetch-function-happy.c
# Prev: hello-function.c
# Next: fetch-function-error-handling.c

# Build & run:
gcc fetch-function.c -o fetch && ./fetch
*/
