#include <stdio.h>
#include "fat32.h"


int main(int argc, char* argv[])
{
    printf("process: %s\nargc: %d\n\n", argv[0], argc);

    for (int i = 1; i < argc; i++)
    {
        printf("============================================================\n");
        printf("Volume: %s\n", argv[i]);

        printf("\n");
    }

    return 0;
}
