#include <stdio.h>
#include <stdlib.h>

/// File loaded in memory. [bytes] is the content and [size is the size].
typedef struct {
    char* bytes;
    long size;
} RawFile;

/// Returns a [rawFile] at from [filename]. Returns NULL on error.
/// Must be closed with [freeRawFile()].
RawFile* getRawFile(char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;
    if (fseek(file, 0, SEEK_END)) return NULL;
    long size = ftell(file);
    if (size == -1) return NULL;
    char* bytes = malloc(size);
    rewind(file);
    if (!fread(bytes, size, 1, file)) return NULL;
    if (fclose(file)) return NULL;
    RawFile* ret = malloc(sizeof(RawFile));
    ret->bytes = bytes;
    ret->size = size;
    return ret;
}

/// Frees resources of [file] returned by [getRawFile()].
void freeRawFile(RawFile* file) {
    free(file->bytes);
    free(file);
}

/// Returns the file specified by the command line arguments.
/// If bad arguments are passed or issue opening file, print
/// error and return NULL. The returned file must be closed.
RawFile* getFile(int argc, char** argv) {
    if (argc > 2) {
        printf("This program only accepts one argument\n");
        return NULL;
    }
    char* filename = argv[1];
    RawFile* file = getRawFile(filename);
    if (!file) {
        printf("There was an issue reading the file: %s\n", filename);
        return NULL;
    }
    return file;
}

int main(int argc, char** argv) {
    RawFile* file = getFile(argc, argv);
    if (!file) return 1;
    freeRawFile(file);
    return 0;
}
