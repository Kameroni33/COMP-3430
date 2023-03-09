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

// Global Variables ===================================================================

// timing variables for logging
long long startTime, endTime;

// global array of output file descriptors (ordered a-z + other)
char *outputPaths[NUM_OUTPUTS] = { "output/a.txt", "output/b.txt", "output/c.txt", "output/d.txt", "output/e.txt", "output/f.txt", "output/g.txt", "output/h.txt", "output/i.txt", "output/j.txt", "output/k.txt", "output/l.txt", "output/m.txt", "output/n.txt", "output/o.txt", "output/p.txt", "output/q.txt", "output/r.txt", "output/s.txt", "output/t.txt", "output/u.txt", "output/v.txt", "output/w.txt", "output/x.txt", "output/y.txt", "output/z.txt", "output/other.txt" };
// global array of output file descriptors (ordered a-z + other)
FILE *outputFiles[NUM_OUTPUTS];
// global array of output file locks (ordered a-z + other)
pthread_mutex_t outputLocks[NUM_OUTPUTS];

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