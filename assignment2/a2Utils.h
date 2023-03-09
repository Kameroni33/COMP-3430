#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

// global constants
#define NUM_OUTPUTS 27
#define MAX_WORD 100

// globally available variables =======================================================

extern long long startTime, endTime;

extern char *outputPaths[NUM_OUTPUTS];  // initialized by a2Utils.c
extern FILE *outputFiles[NUM_OUTPUTS];  // initialized via 'initializeOutputs' function

// outputFiles methods
void initializeOutputs();  // open all output files for append (file descriptors can be acessed via outputFiles array)
void closeOutputs();       // close all output file descriptors

// inputFile methods
FILE*  determineOutputFile(char word[MAX_WORD]);    // helper method for processFile
void processFile(char *inputPath, int threadSafe);  // 0: no locks, 1: use mutex locks for thread safe

// logging and timing methods
long long timeInMilliseconds(void);
void logInfo(long long start, long long end, int workers, int argc, char *argv[]);
