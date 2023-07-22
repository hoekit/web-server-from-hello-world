#include <stdio.h>
// Given an argument which is a file, fetch it, read it and print it

int fetch(char* fname) {
    // open fname
    FILE *fp = fopen(fname, "r");
    if (!fp) {
        printf("404 Not Found: %s\n", fname);
        return 1;
    }

    // read from fname and print
    char con[100];                          // store content in a char array
    while (fgets(con, 100, fp) != NULL)     // read from file into array
        printf("%s", con);                  // print content

    // close fname
    fclose(fp);
    return 0;
}

int main() {
    char *fname = "-no-such-file-.c";
    return fetch(fname);
}

/*
# This: fetch-function-error-handling.c
# Prev: fetch-function-happy.c
# Next: fetch-cli-error-handling.c

# Build & run:
gcc fetch-function.c -o fetch && ./fetch
*/
