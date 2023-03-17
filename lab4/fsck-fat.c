#include <stdio.h>
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

        printf("BS_jmpBoot: ", mbr.BS_jmpBoot)

        printf("\n");
    }

    return 0;
}
