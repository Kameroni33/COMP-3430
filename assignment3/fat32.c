#define _FILE_OFFSET_BITS 64

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "fat32.h"


void printUsage(void);
void info(char *drive);
void list(char *drive);
void printFileStructure(int drive, fat32BS bs, off_t fat, off_t cluster, int depth);
off_t calcClustAddress(int cluster, fat32BS bs);
void calcFileName(char entryName[12], char fileName[13], int extension);
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

void printUsage(void) {
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

void list(char *driveName) {
    printf("\nreading drive '%s'...\n", driveName);

    int drive;

    off_t rootAddress;
    off_t fatAddress;

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

    if (bootSector.BPB_RsvdSecCnt == 0) {
        printf("WARNING: reserved number of sectors is 0\n");
    }

    // determine memory address of FAT
    fatAddress = bootSector.BPB_RsvdSecCnt * bootSector.BPB_BytesPerSec;

    if (bootSector.BPB_RootEntCnt != 0) {
        printf("WARNING: root entry count is not 0\n");
    }

    // determine memory address of root directory
    rootAddress = calcClustAddress(bootSector.BPB_RootClus, bootSector);

    uint32_t sectorSize = bootSector.BPB_BytesPerSec;
    uint32_t clusterSize = bootSector.BPB_SecPerClus * sectorSize;
    uint32_t entrySize = sizeof(fat32Dir);
    uint32_t entriesPerCluster = clusterSize / entrySize;

    printf("\nSectorSize: %u\n", sectorSize);
    printf("ClusterSize: %u\n", clusterSize);
    printf("Root Cluster: %u\n", bootSector.BPB_RootClus);
    printf("Root Address: 0x%lx\n", rootAddress);
    printf("fat32Dir Size: %u\n", entrySize);
    printf("fat32Dir Entries per Cluster: %u\n", entriesPerCluster);
    printf("FAT Address: 0x%lx\n\n", fatAddress);

    // read directory tree starting at the root
    printFileStructure(drive, bootSector, fatAddress, bootSector.BPB_RootClus, 1);

}

void printFileStructure(int drive, fat32BS bs, off_t fat, off_t cluster, int depth) {

    fat32Dir entry;

    uint32_t sectorSize;
    uint32_t clusterSize;
    uint32_t entrySize;
    uint32_t entriesPerCluster;

    char entryName[12];
    char fileName[13];
    off_t nextCluster;
    off_t newCluster;
    off_t memAddress;

    // get memory address for the cluster
    memAddress = calcClustAddress(cluster, bs);

    sectorSize = bs.BPB_BytesPerSec;
    clusterSize = bs.BPB_SecPerClus * sectorSize;
    entrySize = sizeof(fat32Dir);
    entriesPerCluster = clusterSize / entrySize;

    for (uint32_t i = 0; i < entriesPerCluster; i++) {

        // go to entry location
        lseek(drive, (memAddress + (entrySize * i)), SEEK_SET);
        read(drive, &entry, sizeof(fat32Dir));

        // check first char to see it=f it's an empty entry
        uint32_t firstChar = (int)entry.dir_name[0] & 0xFF;
        // printf("\nFirst Character: 0x%x\n", firstChar);
        if (firstChar != 0xE5 && firstChar != 0x00) {

            // make sure the entry name is valid
            // for (int j = 0; j < 11; j++) {
            //     uint32_t currChar = (int)entry.dir_name[0] & 0xFF;
            //     if (currChar <= 0x20 && currChar != 0x05) {
            //         printf("WARNING: Invalid Entry Name Character\n");
            //     }
            // }

            // get the entry name as a string
            strncpy(entryName, entry.dir_name, 11);
            entryName[11] = '\0';

            
            // if LONG_NAME entry
            if (entry.dir_attr == (ATTR_LONG_NAME)) {
                // do nothing...
            }
            // else if . or .. entry
            // else if (entryName[0] == '.') {
            //     // do nothing...
            // }
            // else if HIDDEN entry
            else if (entry.dir_attr == ATTR_HIDDEN) {
                for (int k = 0; k < depth; k++) printf("-");
                calcFileName(entryName, fileName, 0);
                printf("[hidden] %s\n", fileName);
            }
            // else if SYSTEM entry
            else if (entry.dir_attr == ATTR_ARCHIVE) {
                // for (int k = 0; k < depth; k++) printf("-");
                // calcFileName(entryName, fileName, 0);
                // printf("[system] %s\n", fileName);
            }
            // else if VOLUME entry
            else if (entry.dir_attr == ATTR_VOLUME_ID) {
                calcFileName(entryName, fileName, 0);
                printf("[volume] %s\n", fileName);
            }
            // else if DIRECTORY entry
            else if (entry.dir_attr == ATTR_DIRECTORY) {
                for (int k = 0; k < depth; k++) printf("-");
                calcFileName(entryName, fileName, 0);
                printf("[directory] %s\n", fileName);

                // look up address of next directory
                newCluster = ((uint32_t)(entry.dir_first_cluster_hi) << 16) + (uint32_t)(entry.dir_first_cluster_lo);
                // printf("<< newCluster: %u >>\n", newCluster);

                printFileStructure(drive, bs, fat, newCluster, depth+1);
            }
            // else if ARCHIVE entry
            else if (entry.dir_attr == ATTR_ARCHIVE) {
                for (int k = 0; k < depth; k++) printf("-");
                calcFileName(entryName, fileName, 0);
                printf("[archive] %s\n", fileName);
            }
            // else FILE entry
            else {
                for (int k = 0; k < depth; k++) printf("-");
                calcFileName(entryName, fileName, 1);
                printf("[file] %s (%u bytes)\n", fileName, entry.dir_file_size);
            }
        }
    }

    // check if there is another cluster to read for this directory
    lseek(drive, (fat + cluster), SEEK_SET);
    read(drive, &nextCluster, sizeof(uint32_t));

    if (nextCluster >= 0x0FFFFFF8) {
        // printf("end of cluster...\n");
    }
    else {
        printf("proceeding to next cluster... [0x%lx]\n", nextCluster);
        printFileStructure(drive, bs, fat, nextCluster, depth+1);
    }
}

off_t calcClustAddress(int cluster, fat32BS bs) {
    int firstDataSector = bs.BPB_RsvdSecCnt + (bs.BPB_NumFATs * bs.BPB_FATSz32);
    off_t address = (((cluster - 2) * bs.BPB_SecPerClus) + firstDataSector) * bs.BPB_BytesPerSec;
    return address;
}

void calcFileName(char entryName[12], char fileName[13], int extension) {
    int index = 0;
    int length = extension ? 8 : 11;

    // add name
    for (int i = 0; i < length; i++) {
        if (entryName[i] != ' ') {
            fileName[index] = entryName[i];
            index++;
        }
    }

    // add extension
    if (extension) {
        fileName[index] = '.';
        index++;
        for (int i = 8; i < 11; i++) {
            if (entryName[i] != ' ') {
                fileName[index] = entryName[i];
                index++;
            }
        }
    }
    
    // add end of string character
    fileName[index] = '\0';
}


void get(char *drive, char *file) {
    printf("\nGET\n drive: %s\n file:  %s\n\n", drive, file);
}
