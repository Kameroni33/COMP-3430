#include <stdio.h>
#include <stdlib.h>

#define NUM_OUTPUTS 27
#define MAX_WORD 100

void helloWorld();

void initializeOutputs(FILE *outputFiles[NUM_OUTPUTS], char *outputPaths[NUM_OUTPUTS]);
void closeOutputs(FILE *outputFiles[NUM_OUTPUTS]);
FILE*  findOutputFile(FILE *outputFiles[NUM_OUTPUTS], char word[MAX_WORD]);
