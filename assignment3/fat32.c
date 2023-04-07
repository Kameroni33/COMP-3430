#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "fat32.h"

#define _FILE_OFFSET_BITS 64


void printUsage();
void info(char *drive);
void list(char *drive);
void get(char *drive, char *file);

int main(int argc, char *argv[]) {

    // determine which function to run
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
        printf("ERROR: incorrect usage.\n");
        printUsage();
    }

    printf("\nEnd of Process.\n");
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
    printf("  get    fetch and return a file from the drive. Files will be copied to a ./downloads folder in current directory. Requires an additional argument {file}.\n\n");
}

void info(char *driveName) {
    printf("\nreading drive '%s'...\n\n", driveName);

    int drive;

    fat32BS bootSector;
    fat32FSInfo fileSysInfo;

    // open the drive
    if ( (drive = open(driveName, O_RDONLY)) > 0) {
        printf("ERROR: unable to open drive '%s'.\n", driveName);
        exit(1);
    }

    // read Boot Sector (BS)
    lseek(drive, 0, SEEK_SET);
    read(drive, &bootSector, sizeof(fat32BS));

    // read File System Info (FSInfo)
    lseek(drive, (bootSector.BPB_FSInfo * bootSector.BPB_BytesPerSec), SEEK_SET);
    read(drive, &fileSysInfo, sizeof(fat32FSInfo));

    // print information about the drive
    printf("OEM Name:      %s\n", bootSector.BS_OEMName);
    printf("Volume Label:  %s\n", bootSector.BS_VolLab);
    printf("Free Clusters: %d\n", fileSysInfo.free_count);
    printf("Free Space:    %dkB\n", fileSysInfo.free_count * (bootSector.BPB_BytesPerSec * bootSector.BPB_SecPerClus) / 1000);

}

void list(char *drive) {
    printf("\nLIST\n drive: %s\n\n", drive);
}

void get(char *drive, char *file) {
    printf("\nGET\n drive: %s\n file:  %s\n\n", drive, file);
}
