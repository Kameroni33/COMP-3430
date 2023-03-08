#include <stdio.h>  // fopen, fclose

// Global Constants ===================================================================

#define NUM_OUTPUTS 27
#define MAX_WORD 100

// Variables & Structs ================================================================

void helloWorld()
{
    printf("hello world\n");
}

void initializeOutputs(FILE *outputFiles[NUM_OUTPUTS], char *outputPaths[NUM_OUTPUTS])
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

void closeOutputs(FILE *outputFiles[NUM_OUTPUTS])
{
    for ( int i = 0; i < NUM_OUTPUTS; i++)
    {
        if ( fclose(outputFiles[i]) != 0 )
        {
            printf("Error: unable to close file\n");
            exit(1);
        }
    }
}

