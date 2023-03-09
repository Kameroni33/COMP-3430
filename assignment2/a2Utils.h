#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

// Global Constants ===================================================================

#define NUM_OUTPUTS 27  // number of output files (a-z + other)
#define BUFFER_SIZE 10  // size of Job Buffer

#define MAX_WORD 100    // max character count for words in input files
#define MAX_NAME 50     // max character count for input file names

// Externally Available Variables =====================================================

extern long long startTime, endTime;  // should be initalized in 'main'

extern char *outputPaths[NUM_OUTPUTS];  // initialized by a2Utils.c
extern FILE *outputFiles[NUM_OUTPUTS];  // initialized via 'initializeOutputs' function
extern char *fifosPaths[NUM_OUTPUTS]    // initialized via 'initializeFifos' function
extern int  *writers[NUM_OUTPUTS];      // initialized via 'initializeWriters' function

extern int stopThreads;  // initalized to 0 by a2Utils.c and should be set to 1 in 'main'
extern int numJobs;      // number of jobs in jobBuffer

extern pthread_cond_t newJob, aquiredJob;  // condition variables for jobBuffer
extern pthread_mutex_t bufferLock;         // mutex lock for jobBuffer

// Output File Methods ================================================================

void initializeOutputs();  // open all output files for append (file descriptors can be acessed via outputFiles array)
void closeOutputs();       // close all output file descriptors

// Input File Methods =================================================================

FILE*  determineOutputFile(char word[MAX_WORD]);    // helper method for processFile
void processFile(char *inputPath, int threadSafe);  // 0: no locks, 1: use mutex locks for thread safe

// Buffer Methods =====================================================================

void put(char *file);     // add item to the buffer
void get(char *nextJob);  // remove next item from the buffer

// Fifo Methods =======================================================================

void initializeFifos();

// Thread Methods =====================================================================

void *worker(void *arg);  // function for worker threads

// Process Methods ====================================================================

void initalizeWriters();
void writer();

// Logging and Timing Methods =========================================================

long long timeInMilliseconds(void);
void logInfo(long long start, long long end, int workers, int argc, char *argv[]);
