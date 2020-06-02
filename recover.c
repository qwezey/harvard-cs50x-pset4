#include <stdio.h>
#include <stdlib.h>

/// Returns the filename from given command line arguments.
/// If bad arguments are passed, print error and return NULL.
char* parseFilename(int argc, char** argv) {
    if (argc > 2) {
        printf("This program only accepts one argument\n");
        return NULL;
    }
    return argv[1];
}

int main(int argc, char** argv) {
    char* filename = parseFilename(argc, argv);
    if (!filename) return 1;
    return 0;
}
