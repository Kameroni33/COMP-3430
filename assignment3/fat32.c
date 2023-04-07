#include <stdio.h>
#include <string.h>
#include "fat32.h"

#define _FILE_OFFSET_BITS 64


void printUsage();
void info(char *drive);
void list(char *drive);
void get(char *drive, char *file);

int main(int argc, char *argv[]) {

    if (argc >= 3) {
        if (strcmp(argv[2], "info") == 0) {
            info(argv[1]);
        }
        else if (strcmp(argv[2], "list") == 0) {
            list(argv[1]);
        }
        else if (strcmp(argv[2], "get") == 0) {
            if (argc >= 4) {
                get(argv[1], argv[3]);
            }
            else {
                printf("ERROR: no target file specified.\n");
                printUsage();
            }
        }
        else {
            printf("ERROR: invalid command argument '%s'.\n", argv[2]);
            printUsage();
        }
    }

    else {
        printf("Error: incorrect usage.\n");
        printUsage();
    }

    return 0;
}

void printUsage() {
    printf("\nProcess: fat32\n");
    printf("  FAT32 drive reader\n\n");
    printf("Usage:\n");
    printf("  ./fat32 [imagefile] [command] {file}\n\n");
    printf("Available Commands:\n");
    printf("  info   print out some general info about the drive\n");
    printf("  list   print all files and directories on the frive\n");
    printf("  get    fetch and return a file from the drive. Files will be copied to ./output folder in current directory. Requires an additional argument {file}.\n\n");
}

void info(char *drive) {
    printf("\nCommand: info\n  drive: %s\n\n", drive);
}

void list(char *drive) {
    printf("\nCommand: list\n  drive: %s\n\n", drive);
}

void get(char *drive, char *file) {
    printf("\nCommand: get\n  drive: %s\n  file:  %s\n\n", drive, file);
}
