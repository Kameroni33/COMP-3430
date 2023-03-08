#include <stdio.h>

// Global Constants ===================================================================

#define NUM_OUTPUTS 27
#define MAX_WORD 100

// Variables & Structs ================================================================

FILE *outputFiles[NUM_OUTPUTS];
char *outputPaths[NUM_OUTPUTS] = { "output/a.txt", "output/b.txt", "output/c.txt", "output/d.txt", "output/e.txt", "output/f.txt", "output/g.txt", "output/h.txt", "output/i.txt", "output/j.txt", "output/k.txt", "output/l.txt", "output/m.txt", "output/n.txt", "output/o.txt", "output/p.txt", "output/q.txt", "output/r.txt", "output/s.txt", "output/t.txt", "output/u.txt", "output/v.txt", "output/w.txt", "output/x.txt", "output/y.txt", "output/z.txt", "output/other.txt" };

void helloWorld()
{
    printf("hello world\n");
}

void openForAppend(FILE *file, char *path)
{
    
}

void initializeOutputs(FILE *outputFiles[])
{
    for ( int i = 0; i < NUM_OUTPUTS; i++)
    {
        if ((outputFiles[i] = fopen(outputPaths[i], "a")) == NULL)
        {
            printf("Error: unable to open file for append '%s'\n", outputPaths[i]);
            exit(1);
        }
    }
}

