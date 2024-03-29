#include <stdio.h>    // fopen, fclose, fwrite, printf, fscanf
#include <stdlib.h>   // exit

#include "a2Utils.h"  // variables, functions, etc... centeralized


int main(int argc, char *argv[])
{
    startTime = timeInMilliseconds();

    // open all of our output files for 'append'
    initializeOutputs();

    printf("process: %s\nfiles:   %d\n\n", argv[0]+2, argc-1);
    
    if (argc > 1)
    {
        // iterate through all the file names
        for (int i = 1; i < argc; i++)
        {
            printf("processing '%s'\n", argv[i]);
            processFile(argv[i], 0);  // NOT thread-safe option
        }
    }

    else
    {
        printf("Error: no input file(s) provided.\n");
        printf("Usage: ./serial path/to/files/*\n\n");
    }

    // close all of our output files
    closeOutputs();

    endTime = timeInMilliseconds();

    logInfo(endTime - startTime, 1, argv[0]);

    printf("End of Process.\n");
    return 0;
}
