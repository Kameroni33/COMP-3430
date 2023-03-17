#include <stdio.h>
#include <stdlib.h>
#include "fat32.h"


int main(int argc, char* argv[])
{
    printf("process: %s\nargc: %d\n\n", argv[0], argc);

    for (int i = 1; i < argc; i++)
    {
        printf("============================================================\n");
        printf("Volume: %s\n", argv[i]);

        FILE* volume;
        fat32BS mbr = {0};

        if ((volume = fopen(argv[i], "r")) == NULL)
        {
            printf("Error: unable to open volume file");
            exit(1);
        }

        fread(&mbr, sizeof(fat32BS), 1, volume);

        printf("BS_jmpBoot:   0x%X 0x%X 0x%X\n", mbr.BS_jmpBoot[0], mbr.BS_jmpBoot[1], mbr.BS_jmpBoot[2]);
        printf("BPB_FATSz32:  %d\n", mbr.BPB_FATSz32);
        printf("BPB_RootClus: %d\n", mbr.BPB_RootClus);
        printf("BPB_TotSec32: %d\n", mbr.BPB_TotSec32);
        printf("BPB_reserved: %X %X %X %X %X %X %X %X %X %X %X %X\n", mbr.BPB_reserved[0], mbr.BPB_reserved[1], mbr.BPB_reserved[2], mbr.BPB_reserved[3], mbr.BPB_reserved[4], mbr.BPB_reserved[5], mbr.BPB_reserved[6], mbr.BPB_reserved[7], mbr.BPB_reserved[8], mbr.BPB_reserved[9], mbr.BPB_reserved[10], mbr.BPB_reserved[11]);

        printf("\n");
    }

    return 0;
}
