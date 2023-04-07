#include <stdio.h>
#include <string.h>
#include "fat32.h"

#define _FILE_OFFSET_BITS 64


void info(char *drive) {
    printf("info\n");
}

void list(char *drive) {
    printf("list\n");
}

void get(char *drive, char *file) {
    printf("list\n");
}


int main(int argc, char *argv[]) {
    printf("argc: %d\nargv[0]: %s\n\n", argc, argv[0]);

    if (argc >= 3) {
        if (strcmp(argv[2], "info")) {
            info(argv[1]);
        }
        else if (strcmp(argv[2], "list")) {
            list(argv[1]);
        }
        else if (strcmp(argv[2], "get")) {
            if (argc >= 4) {
                get(argv[1], argv[3]);
            }
            else {
                printf("Error: No file specified.\n", argv[2]);
            }
        }
        else {
            printf("Error: Invalid command argument '%s'.\n", argv[2]);
        }
    }

    else {
        printf("Process: fat32\n");
        printf("  FAT32 drive reader\n\n");
        printf("Usage:\n");
        printf("  ./fat32 [imagefile] [command] {file}\n\n");
        printf("Available Commands:\n");
        printf("  info   print out some general info about the drive\n");
        printf("  list   print all files and directories on the frive\n");
        printf("  get    fetch and return a file from the drive. Files will be copied to ./output folder in current directory. Requires an additional argument {file}.\n\n");
    }

    return 0;
}
