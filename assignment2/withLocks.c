#include <stdio.h>    // fopen, fclose, fwrite, printf, fscanf
#include <stdlib.h>   // exit
#include <pthread.h>  // 

#include "a2Utils.h"  // initializeOutputs

// Global Constants ===================================================================

#define MAX_BUFFER 10

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

// shared memory Job Buffer for holding available jobs
FILE *jobBuffer[MAX_BUFFER];
int putNext = 0;  // index to put next job into
int getNext = 0;  // index to get next job from
int numJobs = 0;  // number of jobs in buffer

int stopThreads = 0;  // flag for threads to exit

cond_t newJob, aquiredJob;  // condition variables for buffer
mutex_t bufferLock;         // mutex lock for buffer

// Buffer Methods =====================================================================

void put(FILE *file)
{
    jobBuffer[putNext] = file;             // add new file to the buffer
    putNext = (putNext + 1) % MAX_BUFFER;  // increment & wrap if reached MAX_BUFFER
    numJobs++;                             // update number of jobs in buffer
}

FILE* get()
{
    FILE *nextJob = buffer[getNext];       // get next file from the buffer
    getNext = (getNext + 1) % MAX_BUFFER;  // increment & wrap if reached MAX_BUFFER
    numJobs--;                             // update number of jobs in buffer
    return nextJob;
}

// Thread Methods =====================================================================

void *worker(void *arg)
{
    // run indefinitely until signaled to exit (ie. 'stopThreads' flag)
    while (1)
    {
        // aquire the lock for the jobBuffer
        pthread_mutex_lock(&bufferLock);
        while (numJobs == 0)
        {
            if (stopThreads)  // check for flag to exit
            {
                pthread_exit(NULL);
            }

            // wait until we get notified of a new job
            pthread_cond_wait(&newJob, &bufferLock);
        }

        // get the next file to process & store in temporary variable on this thread
        FILE* tmpFile = get();
        pthread_cond_signal(&aquiredJob);
        pthread_mutex_unlock()
    }
}



// Main Method ========================================================================

int main(int argc, char *argv[]) {

    // Local Variables
    int workers = (int)argv[1];
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
