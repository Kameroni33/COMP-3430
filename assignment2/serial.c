#include <stdio.h>    // fopen, fclose, printf, fscanf
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

int main(int argc, char *argv[]) {

    // Local Variables
    FILE *outputFiles[NUM_OUTPUTS];
    FILE *inputFile;
    char word[MAX_WORD];

    // open all of our output files for 'append'
    initializeOutputs(outputFiles, outputPaths);

    printf("process: %s\nfiles:   %d\n\n", argv[0]+2, argc-1);
    
    if (argc > 1)
    {
        // iterate through all the file names
        for (int i = 1; i < argc; i++)
        {
            // open file and check for errors
            if ((inputFile = fopen(argv[i], "r")) == NULL)
            {
                printf("Error: unable to open file '%s'.\n", argv[i]);
                exit(1);
            }

            // scan through entire file
            while (fscanf(inputFile, "%s", word) != EOF)
            {
                printf("%s\n", word);
                findOutputFile(outputFiles, word);
            }

            // close current file
            fclose(inputFile);
        }
    }

    else
    {
        printf("Error: no input file(s) provided.\n");
        printf("Usage: ./serial path/to/files/*\n\n");
    }

    // close all of our output files
    closeOutputs(outputFiles);

    printf("End of Process.\n");
    return 0;
}
