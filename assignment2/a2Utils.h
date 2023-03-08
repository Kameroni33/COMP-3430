#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// global constants
#define NUM_OUTPUTS 27
#define MAX_WORD 100

// output file methods
void initializeOutputs(FILE *outputFiles[NUM_OUTPUTS], char *outputPaths[NUM_OUTPUTS]);
void closeOutputs(FILE *outputFiles[NUM_OUTPUTS]);
FILE*  determineOutputFile(FILE *outputFiles[NUM_OUTPUTS], char word[MAX_WORD]);

// logging and timing methods
long long timeInMilliseconds(void);
