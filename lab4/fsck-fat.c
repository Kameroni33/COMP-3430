#include <stdio.h>
#include <stdlib.h>
#include "fat32.h"


int main(int argc, char* argv[])
{
    printf("process: %s\nargc: %d\n", argv[0], argc);

    for (int i = 1; i < argc; i++)
    {
        printf("\n==================================================\n");
        printf("Volume: %s\n", argv[i]);
        printf("==================================================\n\n");

        FILE* volume;
        fat32BS bootSector;
        FSInfo fsInfoSector; 
        char FAT[2];

        int validBootSector = 1;

        // open .img file
        if ((volume = fopen(argv[i], "r")) == NULL)
        {
            printf("Error: unable to open volume file");
            exit(1);
        }

        // read Boot Sector
        fread(&bootSector, sizeof(fat32BS), 1, volume);

        // print bootSector info
        printf("BS_jmpBoot:   0x%X 0x%X 0x%X\n", bootSector.BS_jmpBoot[0] & 0xff, bootSector.BS_jmpBoot[1] & 0xff, bootSector.BS_jmpBoot[2] & 0xff);
        printf("BPB_FATSz32:  %d\n", bootSector.BPB_FATSz32);
        printf("BPB_RootClus: %d\n", bootSector.BPB_RootClus);
        printf("BPB_TotSec32: %d\n", bootSector.BPB_TotSec32);
        printf("BPB_reserved: %X %X %X %X %X %X %X %X %X %X %X %X\n\n", bootSector.BPB_reserved[0], bootSector.BPB_reserved[1], bootSector.BPB_reserved[2], bootSector.BPB_reserved[3], bootSector.BPB_reserved[4], bootSector.BPB_reserved[5], bootSector.BPB_reserved[6], bootSector.BPB_reserved[7], bootSector.BPB_reserved[8], bootSector.BPB_reserved[9], bootSector.BPB_reserved[10], bootSector.BPB_reserved[11]);

        // validate BS_jmpBoot
        if ((int)(bootSector.BS_jmpBoot[0] & 0xff) != 0xEB && (int)(bootSector.BS_jmpBoot[0] & 0xff) != 0xE9)
        {
            printf("Inconsistent file system: BS_jmpBoot[0] must be 0xEB or 0xE9, but is 0x%X.\n", bootSector.BS_jmpBoot[0]);
            validBootSector = 0;
        }

        // validate BPB_FATSz32
        if (bootSector.BPB_FATSz32 == 0)
        {
            printf("Inconsistent file system: BPB_FATSz32 should be non-zero, but is%d.\n", bootSector.BPB_FATSz32);
            validBootSector = 0;
        }

        // validate BPB_RootClus
        if (bootSector.BPB_RootClus < 2)
        {
            printf("Inconsistent file system: BPB_RootClus should greater than 2, but is %d.\n", bootSector.BPB_RootClus);
            validBootSector = 0;
        }

        // validate BPB_TotSec32
        if (bootSector.BPB_TotSec32 < 65525)
        {
            printf("Inconsistent file system: BPB_TotSec32 should be greater than 65525, but is %d.\n", bootSector.BPB_TotSec32);
            validBootSector = 0;
        }

        // validate BPB_reserved
        if (bootSector.BPB_reserved[0] != 0 || bootSector.BPB_reserved[1] != 0 || bootSector.BPB_reserved[2] != 0 || bootSector.BPB_reserved[3] != 0 || bootSector.BPB_reserved[4] != 0 || bootSector.BPB_reserved[5] != 0 || bootSector.BPB_reserved[6] != 0 || bootSector.BPB_reserved[7] != 0 || bootSector.BPB_reserved[8] != 0 || bootSector.BPB_reserved[9] != 0 || bootSector.BPB_reserved[10] != 0 || bootSector.BPB_reserved[11] != 0)
        {
            printf("Inconsistent file system: BPB_reserved should all be 0, but is %X %X %X %X %X %X %X %X %X %X %X %X\n", bootSector.BPB_reserved[0], bootSector.BPB_reserved[1], bootSector.BPB_reserved[2], bootSector.BPB_reserved[3], bootSector.BPB_reserved[4], bootSector.BPB_reserved[5], bootSector.BPB_reserved[6], bootSector.BPB_reserved[7], bootSector.BPB_reserved[8], bootSector.BPB_reserved[9], bootSector.BPB_reserved[10], bootSector.BPB_reserved[11]);
            validBootSector = 0;
        }

        // if all validations passed, then print success
        if (validBootSector)
        {
            printf("MBR appears to be consistent.\n");
        }

        // printf("BPB_BytesPerSec: %d\n", bootSector.BPB_BytesPerSec);
        // printf("BPB_FSInfo:      %d\n", bootSector.BPB_FSInfo);
        // printf("BPB_RsvdSecCnt:  %d\n", bootSector.BPB_RsvdSecCnt);

        // read File System Info Sector
        fseek(volume, (bootSector.BPB_FSInfo * bootSector.BPB_BytesPerSec), SEEK_SET);
        fread(&fsInfoSector, sizeof(FSInfo), 1, volume);

        // read File Allocation Table
        fseek(volume, (bootSector.BPB_RsvdSecCnt * bootSector.BPB_BytesPerSec), SEEK_SET);
        fread(&FAT, sizeof(FAT), 1, volume);

        // print FAT info
        printf("\n");
        printf("FAT[0]: 0x%X\n", bootSector.BS_jmpBoot[0]);
        printf("FAT[1]: 0x%X\n\n", bootSector.BS_jmpBoot[1]);

        // validate FAT[0]
        if ((int)FAT[0] != 0x0FFFFFF8)
        {
            printf("Inconsistent file system: FAT[0] should be 0x0FFFFFF8, but is 0x%X\n", FAT[0]);
        }

        // validate FAT[1]
        if ((FAT[1] & 0x0FFFFFFF) == 0x0FFFFFFF)
        {
            printf("Inconsistent file system: FAT[1] should be 0x0FFFFFFF, but is 0x%X\n", FAT[1]);
        }

    }

    printf("\n\nEnd of Process.\n");
    return 0;
}