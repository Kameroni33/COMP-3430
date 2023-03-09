#include <stdio.h>    // fopen, fclose, fwrite, printf, fscanf
#include <stdlib.h>   // exit

#include "a2Utils.h"  // initializeOutputs

// Global Variables ===================================================================

char *outputPaths[NUM_OUTPUTS] = {
    "output/a.txt",
    "output/b.txt",
    "output/c.txt",
    "output/d.txt",
    "output/e.txt",
    "output/f.txt",
    "output/g.txt",
    "output/h.txt",
    "output/i.txt",
    "output/j.txt",
    "output/k.txt",
    "output/l.txt",
    "output/m.txt",
    "output/n.txt",
    "output/o.txt",
    "output/p.txt",
    "output/q.txt",
    "output/r.txt",
    "output/s.txt",
    "output/t.txt",
    "output/u.txt",
    "output/v.txt",
    "output/w.txt",
    "output/x.txt",
    "output/y.txt",
    "output/z.txt",
    "output/other.txt"
};

// global array of output file descriptors (ordered to match outputPaths)
FILE *outputFiles[NUM_OUTPUTS];

// timing variables for logging
long long startTime, endTime;

// Main Method ========================================================================

int main(int argc, char *argv[]) {

    // Local Variables
    FILE *inputFile;
    char word[MAX_WORD];

    startTime = timeInMilliseconds();

    // open all of our output files for 'append'
    initializeOutputs(outputFiles, outputPaths);

    printf("process: %s\nfiles:   %d\n\n", argv[0]+2, argc-1);
    
    if (argc > 1)
    {
        // iterate through all the file names
        for (int i = 1; i < argc; i++)
        {
            printf("reading file '%s'\n", argv[i]);
            processFile(inputFile, argv[i], outputFiles);
        }
    }

    else
    {
        printf("Error: no input file(s) provided.\n");
        printf("Usage: ./serial path/to/files/*\n\n");
    }

    // close all of our output files
    closeOutputs(outputFiles);

    endTime = timeInMilliseconds();

    logInfo(startTime, endTime, 1, argc, argv);

    printf("End of Process.\n");
    return 0;
}
