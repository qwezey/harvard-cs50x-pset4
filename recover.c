#include <stdio.h>
#include <stdlib.h>

/// A "lost" raw JPEG file defined in this assignment.
/// [bytes] is the bytes of the file, and [size] is the size.
typedef struct {
    char* bytes;
    long size;
} RawJpegFile;

/// Returns a raw JPEG file at path [filename]. Returns NULL on error.
/// Must be closed with [closeRawJpegFile()].
RawJpegFile* openRawJpegFile(char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;
    if (fseek(file, 0, SEEK_END)) return NULL;
    long size = ftell(file);
    if (size == -1) return NULL;
    char* bytes = malloc(size);
    rewind(file);
    if (!fread(bytes, size, 1, file)) return NULL;
    if (fclose(file)) return NULL;
    RawJpegFile* ret = malloc(sizeof(RawJpegFile));
    ret->bytes = bytes;
    ret->size = size;
    return ret;
}

/// Closes resources of [file] returned by [openRawJpegFile()].
void closeRawJpegFile(RawJpegFile* file) {
    free(file->bytes);
    free(file);
}

/// Returns the file specified by the command line arguments.
/// If bad arguments are passed or issue opening file, print
/// error and return NULL. The returned file must be closed.
RawJpegFile* getFile(int argc, char** argv) {
    if (argc > 2) {
        printf("This program only accepts one argument\n");
        return NULL;
    }
    char* filename = argv[1];
    RawJpegFile* file = openRawJpegFile(filename);
    if (!file) {
        printf("There was an issue reading the file: %s\n", filename);
        return NULL;
    }
    return file;
}

int main(int argc, char** argv) {
    RawJpegFile* file = getFile(argc, argv);
    if (!file) return 1;
    closeRawJpegFile(file);
    return 0;
}
