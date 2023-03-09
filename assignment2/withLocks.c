#include <stdio.h>    // fopen, fclose, fwrite, printf, fscanf
#include <stdlib.h>   // exit, atoi
#include <string.h>   // strcpy
#include <pthread.h>  // pthread stuff

#include<unistd.h>    // sleep

#include "a2Utils.h"  // initializeOutputs

// Global Constants ===================================================================

#define BUFFER_SIZE 10  // size of Job Buffer
#define MAX_NAME 25     // max characters for input file names

// Global Variables ===================================================================

// timing variables for logging
long long startTime, endTime;

// shared memory Job Buffer for holding available jobs
char jobBuffer[BUFFER_SIZE][MAX_NAME];
int putNext = 0;  // index to put next job into
int getNext = 0;  // index to get next job from
int numJobs = 0;  // number of jobs in buffer

int stopThreads = 0;  // flag for threads to exit

pthread_cond_t newJob, aquiredJob;  // condition variables for buffer
pthread_mutex_t bufferLock;         // mutex lock for buffer

// Buffer Methods =====================================================================

void put(char *file)
{
    strcpy(jobBuffer[putNext], file);       // add new file to the buffer
    putNext = (putNext + 1) % BUFFER_SIZE;  // increment & wrap if reached BUFFER_SIZE
    numJobs++;                              // update number of jobs in buffer
}

void get(char *nextJob)
{
    strcpy(nextJob, jobBuffer[getNext]);    // get next file from the buffer
    getNext = (getNext + 1) % BUFFER_SIZE;  // increment & wrap if reached BUFFER_SIZE
    numJobs--;                              // update number of jobs in buffer
}

// Thread Methods =====================================================================

void *worker(void *arg)
{
    char inputFile[MAX_NAME];  // input file currently in process (local copy)

    // run indefinitely until signaled to exit (ie. 'stopThreads' flag)
    while (1)
    {
        pthread_mutex_lock(&bufferLock);  // aquire the lock for the jobBuffer
        while (numJobs == 0)
        {
            if (stopThreads)  // check for flag to exit
            {
                printf("thread exiting...\n");
                pthread_exit(NULL);
            }
            // wait until we get notified of a new job
            pthread_cond_wait(&newJob, &bufferLock);
        }

        get(inputFile);  // get the next file to process & store in temporary variable

        pthread_cond_signal(&aquiredJob);   // signal main thread that we now have our file
        pthread_mutex_unlock(&bufferLock);  // release lock on the jobBuffer

        printf("processing '%s' from buffer\n", inputFile);

        processFile(inputFile, 1);
    }
}

// Main Method ========================================================================

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Error: too few arguments provided.\n");
        printf("Usage: ./serial num-workers path/to/files/*\n\n");
        exit(1);
    }

    int numWorkers = atoi(argv[1]);  // number of workers
    pthread_t workers[numWorkers];   // array of worker threads

    if (numWorkers < 1)
    {
        printf("Error: invalid number of workers.\n");
        printf("Usage: ./serial num-workers path/to/files/*\n\n");
        exit(1);
    }

    printf("process: %s\nfiles:   %d\nworkers: %d\n", argv[0]+2, argc-1, numWorkers);

    startTime = timeInMilliseconds();

    // open all of our output files for 'append'
    initializeOutputs(outputFiles, outputPaths);

    for (int i = 0; i < numWorkers; i++)
    {
        // start worker threads
        pthread_create(&workers[i], NULL, &worker, NULL);
    }

    // iterate through all the file names
    for (int i = 2; i < argc; i++)
    {
        printf("adding file '%s' to buffer\n", argv[i]);

        pthread_mutex_lock(&bufferLock);  // aquire the lock for the jobBuffer
        while (numJobs == BUFFER_SIZE)
        {
            // wait until a job is aquired (free space in buffer)
            pthread_cond_wait(&aquiredJob, &bufferLock);
        }

        put(argv[i]);  // put the next file to process into the buffer

        pthread_cond_signal(&newJob);       // signal worker threads that there are new jobs
        pthread_mutex_unlock(&bufferLock);  // release lock on the jobBuffer
    }

    stopThreads = 1;                  // set thread exit flag to 'true' (ie. 1)
    pthread_cond_broadcast(&newJob);  // there isn't actually a new job, but should wake all threads to check the stop condition

    for (int i = 0; i < numWorkers; i++)
    {
        printf("signal")
        pthread_cond_signal(&newJob);
    }

    for (int i = 0; i < numWorkers; i++)
    {
        printf("waiting for thread to exit...");
        // wait for worker threads to exit
        pthread_join(workers[i], NULL);
    }

    // close all of our output files
    closeOutputs(outputFiles);

    endTime = timeInMilliseconds();

    logInfo(startTime, endTime, numWorkers, argc, argv);

    printf("End of Process.\n");
    return 0;
}
