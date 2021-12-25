#include <stdio.h>
// Given a CLI argument which is a filename, print it's contents

int fetch(char* fname) {
    // open fname
    FILE *fp = fopen(fname, "r");
    if (!fp) {
        printf("fetch: %s: 404 Not Found\n", fname);
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

int main(int argc, char *argv[]) {
    if ( argc < 2 ) {
        printf("\n  USAGE: fetch FILE\n\n");
        return 1;
    }

    return fetch(argv[1]);
}

/*
# This: fetch-cli-error-handling.c
# Prev: fetch-function-error-handling.c
# Next: learn-about-sockets.md

# Build & run:
gcc fetch-function.c -o fetch && ./fetch
*/
