#include <stdio.h>
#include <stdlib.h>

/// Returns the file specified by the command line arguments.
/// If bad arguments are passed or issue opening file, print
/// error and return NULL. The returned file must be closed.
FILE* getFile(int argc, char** argv) {
    if (argc > 2) {
        printf("This program only accepts one argument\n");
        return NULL;
    }
    char* filename = argv[1];
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("There was an issue reading the file: %s\n", filename);
        return NULL;
    }
    return file;
}

int main(int argc, char** argv) {
    FILE* file = getFile(argc, argv);
    if (!file) return 1;
    fclose(file);
    return 0;
}
