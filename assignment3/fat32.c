#define _FILE_OFFSET_BITS 64

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "fat32.h"


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
    printf("\nreading drive '%s'...\n", driveName);

    int drive;
    char volLabel[12];
    char FileSysType[9];
    float clusterSize;
    float freeSpace;

    fat32BS bootSector;
    fat32FSInfo fileSysInfo;

    // open the drive
    if ( (drive = open(driveName, O_RDONLY)) < 0) {
        printf("ERROR: unable to open drive '%s'.\n", driveName);
        exit(1);
    }

    // read Boot Sector (BS)
    lseek(drive, 0, SEEK_SET);
    read(drive, &bootSector, sizeof(fat32BS));

    // read File System Info (FSInfo)
    lseek(drive, (bootSector.BPB_FSInfo * bootSector.BPB_BytesPerSec), SEEK_SET);
    read(drive, &fileSysInfo, sizeof(fat32FSInfo));

    // get volume label and file system type strings
    strncpy(volLabel, bootSector.BS_VolLab, 11);
    strncpy(FileSysType, bootSector.BS_FilSysType, 8);
    volLabel[11] = '\0';
    FileSysType[8] = '\0';

    // determin cluster size and free space
    clusterSize = ((float)bootSector.BPB_BytesPerSec * (float)bootSector.BPB_SecPerClus) / 1024;
    freeSpace = (float)fileSysInfo.free_count * clusterSize;

    // print information about the drive
    printf("\nOEM Name: %s\n", bootSector.BS_OEMName);
    printf("Volume ID: %u\n", bootSector.BS_VolID);
    printf("Volume Label: %s\n", volLabel);
    printf("File System Type: %s\n", FileSysType);
    printf("Bytes per Sector: %u\n", bootSector.BPB_BytesPerSec);
    printf("Sectors per Cluster: %u\n", bootSector.BPB_SecPerClus);
    printf("Cluster Size: %.3fkB\n", clusterSize);
    printf("Free Clusters: %u\n", fileSysInfo.free_count);
    printf("Free Space: %.1fkB\n", freeSpace);
}

void printFileStructure() {

}

void list(char *driveName) {
    printf("\nreading drive '%s'...\n", driveName);

    int drive;
    int firstDataSector;

    off_t rootAddress;
    off_t fatAddress;

    fat32BS bootSector;
    fat32FSInfo fileSysInfo;
    fat32Dir dirEntry;

    // open the drive
    if ( (drive = open(driveName, O_RDONLY)) < 0) {
        printf("ERROR: unable to open drive '%s'.\n", driveName);
        exit(1);
    }

    // read Boot Sector (BS)
    lseek(drive, 0, SEEK_SET);
    read(drive, &bootSector, sizeof(fat32BS));

    // read File System Info (FSInfo)
    lseek(drive, (bootSector.BPB_FSInfo * bootSector.BPB_BytesPerSec), SEEK_SET);
    read(drive, &fileSysInfo, sizeof(fat32FSInfo));

    if (bootSector.BPB_RsvdSecCnt == 0) {
        printf("WARNING: reserved number of sectors is 0\n");
    }

    // determine memory address of FAT
    fatAddress = bootSector.BPB_RsvdSecCnt * bootSector.BPB_BytesPerSec;

    if (bootSector.BPB_RootEntCnt == 0) {
        printf("WARNING: root entry count is 0\n");
    }

    // determine memory address of root directory
    firstDataSector = bootSector.BPB_RsvdSecCnt + (bootSector.BPB_NumFATs * bootSector.BPB_FATSz32);
    rootAddress = (((bootSector.BPB_RootClus - 2) * bootSector.BPB_SecPerClus) + firstDataSector) * bootSector.BPB_BytesPerSec;

    // read root directory
    lseek(drive, (rootAddress), SEEK_SET);
    read(drive, &dirEntry, sizeof(fat32Dir));

    printf("\nRoot Cluster: %u\n", bootSector.BPB_RootClus);
    printf("Root Address: 0x%lx\n", rootAddress);

    off_t nextCluster;
    lseek(drive, (fatAddress + bootSector.BPB_RootClus), SEEK_SET);
    read(drive, &nextCluster, 32);
    
    printf("Dir Name: %s\n", dirEntry.dir_name);
    printf("Next cluster: %ld, 0x%lx", nextCluster, nextCluster);

}

void get(char *drive, char *file) {
    printf("\nGET\n drive: %s\n file:  %s\n\n", drive, file);
}
