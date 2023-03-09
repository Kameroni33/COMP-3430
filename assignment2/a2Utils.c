#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

// Global Constants ===================================================================

#define NUM_OUTPUTS 27  // number of output files (a-z + other)
#define BUFFER_SIZE 10  // size of Job Buffer

#define MAX_WORD 100    // max character count for words in input files
#define MAX_NAME 50     // max character count for input file names

// Global Variables ===================================================================

// timing variables for logging
long long startTime, endTime;

// global array of output file descriptors (ordered a-z + other)
char *outputPaths[NUM_OUTPUTS] = { "output/a.txt", "output/b.txt", "output/c.txt", "output/d.txt", "output/e.txt", "output/f.txt", "output/g.txt", "output/h.txt", "output/i.txt", "output/j.txt", "output/k.txt", "output/l.txt", "output/m.txt", "output/n.txt", "output/o.txt", "output/p.txt", "output/q.txt", "output/r.txt", "output/s.txt", "output/t.txt", "output/u.txt", "output/v.txt", "output/w.txt", "output/x.txt", "output/y.txt", "output/z.txt", "output/other.txt" };
// global array of output file descriptors (ordered a-z + other)
FILE *outputFiles[NUM_OUTPUTS];
// global array of output file locks (ordered a-z + other)
pthread_mutex_t outputLocks[NUM_OUTPUTS];
// global array of output file writer PIDs
int *writers[NUM_OUTPUTS];

// global array of FIFOs
char *fifoPaths[NUM_OUTPUTS] = { "fifos/aFifo", "fifos/bFifo", "fifos/cFifo", "fifos/dFifo", "fifos/eFifo", "fifos/fFifo", "fifos/gFifo", "fifos/hFifo", "fifos/iFifo", "fifos/jFifo", "fifos/kFifo", "fifos/lFifo", "fifos/mFifo", "fifos/nFifo", "fifos/oFifo", "fifos/pFifo", "fifos/qFifo", "fifos/rFifo", "fifos/sFifo", "fifos/tFifo", "fifos/uFifo", "fifos/vFifo", "fifos/wFifo", "fifos/xFifo", "fifos/yFifo", "fifos/zFifo", "fifos/otherFifo" };

// shared memory Job Buffer for holding available jobs
char jobBuffer[BUFFER_SIZE][MAX_NAME];
int putNext = 0;  // index to put next job into
int getNext = 0;  // index to get next job from
int numJobs = 0;  // number of jobs in buffer

int stopThreads = 0;  // flag for threads to exit

pthread_cond_t newJob, aquiredJob;  // condition variables for buffer
pthread_mutex_t bufferLock;         // mutex lock for buffer

// outputFiles Methods ================================================================

void initializeOutputs()
{
    for ( int i = 0; i < NUM_OUTPUTS; i++)
    {
        if ( (outputFiles[i] = fopen(outputPaths[i], "a")) == NULL )
        {
            printf("Error: unable to open file for append '%s'\n", outputPaths[i]);
            exit(1);
        }
    }
}

void closeOutputs()
{
    for ( int i = 0; i < NUM_OUTPUTS; i++)
    {
        if ( fclose(outputFiles[i]) == EOF )
        {
            printf("Error: unable to close file\n");
            exit(1);
        }
    }
}

// inputFile Methods ==================================================================

int determineOutputFile(char word[MAX_WORD])
{
    // uppercase ASCII letters range from 65 - 90
    if ( word[0] > 64 && word[0] < 91 )
    {
        // return the correct output file index by offsetting by 65
        return word[0]-65;
    }

    // lowercase ASCII letters range from 97 - 122
    else if ( word[0] > 96 && word[0] < 123 )
    {
        // return the correct output file index by offsetting by 97
        return word[0]-97;
    }

    else
    {
        // all other characters go the same output file (last index in our list of files)
        return NUM_OUTPUTS-1;
    }
}

void processFile(char *inputPath, int threadSafe)
{
    FILE *inputFile;      // file descriptor for input file
    int outputIndex;      // index of output file in outputFiles
    char word[MAX_WORD];  // current word in file

    if ((inputFile = fopen(inputPath, "r")) == NULL)  // open input file
    {
        printf("Error: unable to open file '%s'.\n", inputPath);
        exit(1);
    }

    // scan through entire input file
    while (fscanf(inputFile, "%s", word) != EOF)
    {
        outputIndex = determineOutputFile(word);  // determine output file index

        if (threadSafe)  // thread-safe (ie. check for locks)
        {
            pthread_mutex_lock(&outputLocks[outputIndex]);    // aquire lock for output file
            fprintf(outputFiles[outputIndex], "%s\n", word);  // append current word
            pthread_mutex_unlock(&outputLocks[outputIndex]);  // release lock on output file
        }

        else  // thread-unsafe (ie. don't check for locks)
        {
            fprintf(outputFiles[outputIndex], "%s\n", word);  // append current word
        }
        
    }

    if ( fclose(inputFile) == EOF )  // close current input file
    {
        printf("Error: unable to close file '%s'\n", inputPath);
        exit(1);
    }
}

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

// Fifo Methods =======================================================================

void initializeFifos()
{
    for (int i = 0; i < NUM_OUTPUTS; i++)
    {
        mkfifo(fifosPaths[i], S_IRUSR | S_IWUSR);  // make fifo with read/write privilages for user
    }
}

// Thread Methods =====================================================================

void *worker(void *arg)
{
    char inputFile[MAX_NAME];  // input file currently in process (local copy)

    // run indefinitely until signaled to exit (ie. 'stopThreads' flag)
    while (!stopThreads)
    {
        pthread_mutex_lock(&bufferLock);  // aquire the lock for the jobBuffer
        while (numJobs == 0)
        {
            // wait until we get notified of a new job
            pthread_cond_wait(&newJob, &bufferLock);

            if (stopThreads)  // check for flag to exit
            {
                // necessary in case we were sleeping when process finished
                printf("thread exiting...\n");
                pthread_exit(NULL);
            }
        }

        get(inputFile);  // get the next file to process & store in temporary variable

        pthread_cond_signal(&aquiredJob);   // signal main thread that we now have our file
        pthread_mutex_unlock(&bufferLock);  // release lock on the jobBuffer

        printf("processing '%s' from buffer\n", inputFile);

        processFile(inputFile, 1);
    }

    printf("thread exiting...\n");
    pthread_exit(NULL);
}

// Process Methods ====================================================================

void initalizeWriters()
{
    for (int i = 0; i <NUM_OUTPUTS; i++)
    {
        int newPid = fork();  // fork new process

        if (newPid == 0)  // child process
        {
            writer(i);  // ???
        }

        else  // parent process
        {
            writers[i] = newPid;
        }
    }
}

void writer(int index)
{
    FILE *fifo;  // fifo file descriptor

    if ((fifo = fopen(fifoPaths, "r")) == NULL)  // open fifo for reading
    {
        printf("Error: unable to open file '%s'.\n", inputPath);
        exit(1);
    }
}

// Logging and Timing Methods =========================================================

// from good 'ol stack overflow (https://stackoverflow.com/a/44896326)
long long timeInMilliseconds(void) {
    struct timeval timeVal;
    gettimeofday(&timeVal,NULL);
    return (( ( long long )timeVal.tv_sec) * 1000) + (timeVal.tv_usec / 1000);
}

void logInfo(long long start, long long end, int workers, int argc, char *argv[])
{
    FILE *logFile;
    char logName[] = "logs.txt";

    printf("\nlogging info to '%s'\n\n", logName);

    // open log file
    if ( (logFile = fopen(logName, "a")) == NULL )
    {
        printf("Error: unable to open file '%s'\n", logName);
        exit(1);
    }

    // write to log file
    fprintf(logFile, "Program: %s\n", argv[0]+2);
    fprintf(logFile, "Files:   %d\n", argc);
    fprintf(logFile, "Workers: %d\n", workers);
    fprintf(logFile, "Start:   %lld ms\n", start);
    fprintf(logFile, "End:     %lld ms\n", start);
    fprintf(logFile, "Runtime: %lld ms\n\n", end - start);

    // close log file
    if ( fclose(logFile) == EOF )
    {
        printf("Error: unable to close file '%s'\n", logName);
        exit(1);
    }
}