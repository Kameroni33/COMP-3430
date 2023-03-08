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

FILE *outputFiles[NUM_OUTPUTS];

long long startTime;
long long endTime;

// Thread Method ======================================================================



// Main Method ========================================================================

int main(int argc, char *argv[]) {

    // Local Variables
    int workers = argv[1];
    FILE *inputFile;
    char word[MAX_WORD];

    startTime = timeInMilliseconds();

    // open all of our output files for 'append'
    initializeOutputs(outputFiles, outputPaths);

    printf("process: %s\nfiles:   %d\nworkers: %d\n", argv[0]+2, argc-1, workers);
    
    if (argc > 2)
    {
        // iterate through all the file names
        for (int i = 1; i < argc; i++)
        {
            printf("reading file '%s'\n", argv[i]);

            // open input file and check for errors
            if ((inputFile = fopen(argv[i], "r")) == NULL)
            {
                printf("Error: unable to open file '%s'.\n", argv[i]);
                exit(1);
            }

            // scan through entire input file
            while (fscanf(inputFile, "%s", word) != EOF)
            {
                // call our util function to determine which output file to write to and append the current word to it
                fprintf(determineOutputFile(outputFiles, word), "%s\n", word);
            }

            // close current input file
            fclose(inputFile);
        }
    }

    else
    {
        printf("Error: too few arguments provided.\n");
        printf("Usage: ./serial num-workers path/to/files/*\n\n");
    }

    // close all of our output files
    closeOutputs(outputFiles);

    endTime = timeInMilliseconds();

    logInfo(startTime, endTime, workers, argc, argv);

    printf("End of Process.\n");
    return 0;
}
