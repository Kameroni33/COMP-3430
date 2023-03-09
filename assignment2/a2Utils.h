#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/wait.h>

// Global Constants ===================================================================

#define NUM_OUTPUTS 27  // number of output files (a-z + other)
#define BUFFER_SIZE 10  // size of Job Buffer

#define MAX_WORD 100    // max character count for words in input files
#define MAX_NAME 50     // max character count for input file names

// Externally Available Variables =====================================================

extern long long startTime, endTime;  // should be initalized in 'main'

extern char *fileNames[NUM_OUTPUTS];    // initialized by a2Utils.c
extern FILE *outputFiles[NUM_OUTPUTS];  // initialized via 'initializeOutputs' function
extern FILE *fifoFiles[NUM_OUTPUTS];    // initialized via 'initializeFifos' function
extern int  writers[NUM_OUTPUTS];       // initialized via 'initializeWriters' function

extern int stopThreads;  // initalized to 0 by a2Utils.c and should be set to 1 in 'main'
extern int useFifos;     // initalized to 0 by a2Utils.c can be set to 1 in 'main' if FIFOs are desired
extern int numJobs;      // number of jobs in jobBuffer

extern pthread_cond_t newJob, aquiredJob;  // condition variables for jobBuffer
extern pthread_mutex_t bufferLock;         // mutex lock for jobBuffer

// Output File Methods ================================================================

void initializeOutputs();  // open all output files for append (file descriptors can be acessed via outputFiles array)
void closeOutputs();       // close all output file descriptors

// Input File Methods =================================================================

FILE*  determineOutputFile(char word[MAX_WORD]);  // helper method for processFile
void processFile(char *inputPath, int option);    // read file and write to write to appropriate source

// 0: question 1 (no locks, write to file)
// 1: question 2 (thread-safe, write to file)
// 2: question 3 (write to FIFO)

// Buffer Methods =====================================================================

void put(char *file);     // add item to the buffer
void get(char *nextJob);  // remove next item from the buffer

// Fifo Methods =======================================================================

void makeFifos();        // create FIFOs at path given in fifoPaths
void initializeFifos();  // open all FIFOs for write (file descriptors can be acessed via fifoFiles array)
void closeFifos();       // close all FIFO file descriptors

// Thread Methods =====================================================================

void *worker(void *arg);  // function for worker threads

// Process Methods ====================================================================

void initalizeWriters();  // fork NUM_OUTPUTS processes with index values 0 through NUM_OUTPUTS-1
void writer(int index);   // function for writer process

// File Name Methods ==================================================================

char* getOutputFilePath(int index);  // returns char* with full path to output file
char* getFifoFilePath(int index);    // returns char* with full path to fifo file

// Logging and Timing Methods =========================================================

long long timeInMilliseconds(void);
void logInfo(long long start, long long end, int workers, int argc, char *argv[]);
