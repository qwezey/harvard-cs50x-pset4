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
RawFile* parseArgs(int argc, char** argv) {
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

/// Returns NULL if [bytes] doesn't have a JPEG signature, else returns 1.
int hasJpegSignature(char* bytes) {
    return (__uint8_t)bytes[0] == 0xff &&
           (__uint8_t)bytes[1] == 0xd8 &&
           (__uint8_t)bytes[2] == 0xff &&
           ((__uint8_t)bytes[3] & 0xe0) == 0xe0;
}

/// Recovers jpeg files from [rawFile] and saves them to disk.
void recoverJpegs(RawFile* rawFile) {
    char* rawBytes = rawFile->bytes;
    long rawSize = rawFile->size;
    char* end = rawBytes + rawSize;
    int blockSize = 512;
    int fileCount = 0;
    char fileNameBuffer[1024];
    for (; rawBytes < end; rawBytes += blockSize) {
        if (hasJpegSignature(rawBytes)) {
            sprintf(fileNameBuffer, "recovered-%d.jpg", ++fileCount);
            FILE* file = fopen(fileNameBuffer, "w+");
            fwrite(rawBytes, blockSize, 1, file);
            fclose(file);
        }
    }
}

int main(int argc, char** argv) {
    RawFile* file = parseArgs(argc, argv);
    if (!file) return 1;
    recoverJpegs(file);
    freeRawFile(file);
    return 0;
}
