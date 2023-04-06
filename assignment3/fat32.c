#include <stdio.h>
#include "fat32.h"

#define _FILE_OFFSET_BITS 64


void info() {

}

void list() {

}

void get() {

}


int main(int argc, char *argv[]) {
    printf("argc: %d\nargv[0]: %s\n\n", argc, argv[0]);

    if (argc >= 3 && argc <= 4) {
        
    }

    else {
        printf("\n\n  display information about a FAT32 drive\n\n");
        printf("Usage:\n");
        printf("  ./fat32 [imagefile] [command]\n\n");
        printf("Available Commands:\n");
        printf("  info   print out some general info about the drive\n");
        printf("  list   print all files and directories on the frive\n");
        printf("  get    fetch and return a file from the drive. Files will be copied to an ./output folder. Requires an additional parameter [file].\n\n");
    }

    return 0;
}
