#include <stdio.h>    // fopen, fclose, fwrite, printf, fscanf
#include <stdlib.h>   // exit, atoi
#include <string.h>   // strcpy
#include <pthread.h>  // pthread stuff

#include "a2Utils.h"  // variables, functions, etc... centeralized


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
    initializeOutputs();

    for (int i = 0; i < numWorkers; i++)
    {
        printf("starting worker thread (%d)\n", i+1);
        pthread_create(&workers[i], NULL, &worker, NULL);  // start worker threads
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
        printf("waiting for worker thread to exit (%d)\n", i+1);
        pthread_join(workers[i], NULL);  // wait for worker threads to exit
    }

    // close all of our output files
    closeOutputs();

    endTime = timeInMilliseconds();

    logInfo(startTime, endTime, numWorkers, argv[0]);

    printf("End of Process.\n");
    return 0;
}
