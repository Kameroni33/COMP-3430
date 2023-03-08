#include <stdio.h>    // printf, fscanf
#include <stdlib.h>   // exit
#include "a2Utils.h"  // 

#define MAX_WORD 100

int main(int argc, char *argv[]) {

    // Variables
    FILE *file;
    char word[MAX_WORD];

    printf("process: %s\nfiles:   %d\n\n", argv[0][0], argc-1);
    
    if (argc > 1)
    {
        // iterate through all the file names
        for (int i = 1; i < argc; i++)
        {
            // open file and check for errors
            if ((file = fopen(argv[i], "r")) == NULL)
            {
                printf("Error: unable to open file '%s'\n", argv[i]);
                exit(1);
            }

            // scan through entire file
            while (fscanf(file, "%s", word) != EOF)
            {
                printf("%s\n", word);
            }

            // close current file
            fclose(file);
        }
    }

    else
    {
        printf("Usage: ./serial path/to/files/*\n\n");
    }



    printf("End of Process.\n");
    return 0;
}
