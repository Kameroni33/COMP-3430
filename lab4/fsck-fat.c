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
        fat32BS mbr = {0};

        int validMBR = 1;

        // open .img file
        if ((volume = fopen(argv[i], "r")) == NULL)
        {
            printf("Error: unable to open volume file");
            exit(1);
        }

        // read volume into fat32BS struct
        fread(&mbr, sizeof(fat32BS), 1, volume);

        printf("BS_jmpBoot:   0x%X 0x%X 0x%X\n", mbr.BS_jmpBoot[0] & 0xff, mbr.BS_jmpBoot[1] & 0xff, mbr.BS_jmpBoot[2] & 0xff);
        printf("BPB_FATSz32:  %d\n", mbr.BPB_FATSz32);
        printf("BPB_RootClus: %d\n", mbr.BPB_RootClus);
        printf("BPB_TotSec32: %d\n", mbr.BPB_TotSec32);
        printf("BPB_reserved: %X %X %X %X %X %X %X %X %X %X %X %X\n\n", mbr.BPB_reserved[0], mbr.BPB_reserved[1], mbr.BPB_reserved[2], mbr.BPB_reserved[3], mbr.BPB_reserved[4], mbr.BPB_reserved[5], mbr.BPB_reserved[6], mbr.BPB_reserved[7], mbr.BPB_reserved[8], mbr.BPB_reserved[9], mbr.BPB_reserved[10], mbr.BPB_reserved[11]);

        //validate BS_jmpBoot
        if (mbr.BS_jmpBoot[0] != 0xEB && mbr.BS_jmpBoot[0] != 0xE9)
        {
            printf("Inconsistent file system: BS_jmpBoot[0] must be 0xEB or 0xE9, but is 0x%X.\n", mbr.BS_jmpBoot[0]);
            validMBR = 0;
        }

        //validate BPB_FATSz32
        if (mbr.BPB_FATSz32 == 0)
        {
            printf("Inconsistent file system: BPB_FATSz32 should be non-zero, but is%d.\n", mbr.BPB_FATSz32);
            validMBR = 0;
        }

        //validate BPB_RootClus
        if (mbr.BPB_RootClus < 2)
        {
            printf("Inconsistent file system: BPB_RootClus should greater than 2, but is %d.\n", mbr.BPB_RootClus);
            validMBR = 0;
        }

        //validate BPB_TotSec32
        if (mbr.BPB_TotSec32 < 65525)
        {
            printf("Inconsistent file system: BPB_TotSec32 should be greater than 65525, but is %d.\n", mbr.BPB_TotSec32);
            validMBR = 0;
        }

        //validate BPB_reserved
        if (mbr.BPB_reserved[0] != 0 || mbr.BPB_reserved[1] != 0 || mbr.BPB_reserved[2] != 0 || mbr.BPB_reserved[3] != 0 || mbr.BPB_reserved[4] != 0 || mbr.BPB_reserved[5] != 0 || mbr.BPB_reserved[6] != 0 || mbr.BPB_reserved[7] != 0 || mbr.BPB_reserved[8] != 0 || mbr.BPB_reserved[9] != 0 || mbr.BPB_reserved[10] != 0 || mbr.BPB_reserved[11] != 0)
        {
            printf("Inconsistent file system: BPB_reserved should all be 0, but is %X %X %X %X %X %X %X %X %X %X %X %X\n", mbr.BPB_reserved[0], mbr.BPB_reserved[1], mbr.BPB_reserved[2], mbr.BPB_reserved[3], mbr.BPB_reserved[4], mbr.BPB_reserved[5], mbr.BPB_reserved[6], mbr.BPB_reserved[7], mbr.BPB_reserved[8], mbr.BPB_reserved[9], mbr.BPB_reserved[10], mbr.BPB_reserved[11]);
            validMBR = 0;
        }

        if (validMBR)
        {
            printf("MBR appears to be consistent.\n");
        }
    }

    printf("\n\nEnd of Process.\n");
    return 0;
}
