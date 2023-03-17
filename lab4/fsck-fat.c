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

        printf("BS_jmpBoot:   %x\n", mbr.BS_jmpBoot[0]);
        printf("BPB_FATSz32:  %d\n", mbr.BPB_FATSz32);
        printf("BPB_RootClus: %d\n", mbr.BPB_RootClus);
        printf("BPB_TotSec32: %d\n", mbr.BPB_TotSec32);
        printf("BPB_reserved: %s\n", mbr.BPB_reserved);

        printf("\n");
    }

    return 0;
}
