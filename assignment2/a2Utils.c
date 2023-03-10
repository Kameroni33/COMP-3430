#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

// Global Constants ===================================================================

#define NUM_OUTPUTS 27  // number of output files (a-z + other)
#define BUFFER_SIZE 10  // size of Job Buffer

#define MAX_WORD 100    // max character count for words in input files
#define MAX_NAME 100    // max character count for input file names

// Global Variables ===================================================================

const char outputDir[] = "/tmp/ronaldk1/output/";
const char fifoDir[] = "/tmp/ronaldk1/fifo/";
const char logDir[] = "/tmp/ronaldk1/log/";

// timing variables for logging
long long startTime, endTime;

// global array of output file paths (ordered a-z + other)
char *fileNames[NUM_OUTPUTS] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "other" };
// global array of output file descriptors (ordered a-z + other)
FILE *outputFiles[NUM_OUTPUTS];
// global array of output file locks (ordered a-z + other)
pthread_mutex_t outputLocks[NUM_OUTPUTS];

// global array of FIFO file descriptors (ordered a-z + other)
FILE *fifoFiles[NUM_OUTPUTS];

// global array of output file writer PIDs
int writers[NUM_OUTPUTS];

// shared memory Job Buffer for holding available jobs
char jobBuffer[BUFFER_SIZE][MAX_NAME];
int putNext = 0;  // index to put next job into
int getNext = 0;  // index to get next job from
int numJobs = 0;  // number of jobs in buffer

int stopThreads = 0;  // flag for threads to exit
int useFifos = 0;     // for to indicate if we should use FIFOs (0 by default)

pthread_cond_t newJob, aquiredJob;  // condition variables for buffer
pthread_mutex_t bufferLock;         // mutex lock for buffer


// File Name Methods ==================================================================

char* getOutputFilePath(int index)
{
    // get size for the full path name (dir + name + ext)
    int pathLen = strlen(outputDir) + strlen(fileNames[index]) + strlen(".txt");
    char *filePath = malloc(sizeof(char) * pathLen);             // allocate space for file path
    sprintf(filePath, "%s%s.txt", outputDir, fileNames[index]);  // format and set file path
    return filePath;
}

char* getFifoFilePath(int index)
{
    // get size for the full path name (dir + name + 'Fifo')
    int pathLen = strlen(fifoDir) + strlen(fileNames[index]) + strlen("Fifo");
    char *filePath = malloc(sizeof(char) * pathLen);           // allocate space for file path
    sprintf(filePath, "%s%sFifo", fifoDir, fileNames[index]);  // format and set file path
    return filePath;
}

// outputFiles Methods ================================================================

void initializeOutputs()
{
    char *outputFilePath;  // output file name

    for ( int i = 0; i < NUM_OUTPUTS; i++)
    {
        if ( (outputFiles[i] = fopen((outputFilePath = getOutputFilePath(i)), "a")) == NULL )
        {
            printf("Error: unable to open file for append.\n");
            exit(1);
        }

        free(outputFilePath);  // free filename pointer
    }
}

void closeOutputs()
{
    for ( int i = 0; i < NUM_OUTPUTS; i++)
    {
        if ( fclose(outputFiles[i]) == EOF )
        {
            printf("Error: unable to close file.\n");
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

void processFile(char *inputPath, int option)
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

        if (option == 0)  // question 1 (write directly to output file)
        {
            fprintf(outputFiles[outputIndex], "%s\n", word);  // append current word
        }

        else if (option == 1)  // question 2 (thread-safe write to output file)
        {
            pthread_mutex_lock(&outputLocks[outputIndex]);    // aquire lock for output file
            fprintf(outputFiles[outputIndex], "%s\n", word);  // append current word
            pthread_mutex_unlock(&outputLocks[outputIndex]);  // release lock on output file
        }

        else if (option == 2)  // question 3 (write to FIFO)
        {
            fprintf(fifoFiles[outputIndex], "%s\n", word);  // append current word
        }
        
        else
        {
            printf("Error: unexpected option '%d'.", option);
        }
    }

    if ( fclose(inputFile) == EOF )  // close current input file
    {
        printf("Error: unable to close file '%s'.\n", inputPath);
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

void makeFifos()
{
    char *fifoFilePath;    // fifo file name

    for (int i = 0; i < NUM_OUTPUTS; i++)
    {
        mkfifo((fifoFilePath = getFifoFilePath(i)), S_IRUSR | S_IWUSR);  // make fifo with read/write privilages for user
        free(fifoFilePath);  // free filename pointer
    }
}

void initializeFifos()
{
    char *fifoFilePath;    // fifo file name

    for ( int i = 0; i < NUM_OUTPUTS; i++)
    {
        if ( (fifoFiles[i] = fopen((fifoFilePath = getFifoFilePath(i)), "w")) == NULL )
        {
            printf("Error: unable to open fifo.\n");
            exit(1);
        }

        free(fifoFilePath);  // free filename pointer
    }
}

void closeFifos()
{
    for ( int i = 0; i < NUM_OUTPUTS; i++)
    {
        if ( fclose(fifoFiles[i]) == EOF )
        {
            printf("Error: unable to close fifo.\n");
            exit(1);
        }
    }
}

// Thread Methods =====================================================================

void *worker()
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

        if (useFifos)  // write to FIFO (option = 2)
        {
            processFile(inputFile, 2);
        }

        else  // write to file thread-safe (option = 1)
        {
            processFile(inputFile, 1);
        }
    }

    printf("thread exiting...\n");
    pthread_exit(NULL);
}

// Process Methods ====================================================================

void writer(int index)
{
    FILE *fifo;            // fifo file descriptor
    char *fifoFilePath;    // fifo file name
    char *outputFilePath;  // output file name
    char word[MAX_WORD];   // current char from fifo

    if ((fifo = fopen(fifoFilePath = getFifoFilePath(index), "r")) == NULL)  // open fifo for reading
    {
        printf("Error: unable to open fifo for read.\n");
        exit(1);
    }

    free(fifoFilePath);  // free filename pointer

    if ( (outputFiles[index] = fopen((outputFilePath = getOutputFilePath(index)), "a")) == NULL )  // open output file for writing
    {
        printf("Error: unable to open file for append.\n");
        exit(1);
    }

    free(outputFilePath);  // free filename pointer

    // read from FIFO -> write to output file (unitl FIFO is closed)
    while (fscanf(fifo, "%s", word) != EOF)
    {
        fprintf(outputFiles[index], "%s\n", word);  // append current word
    }

    if ( fclose(outputFiles[index]) == EOF )  // open output file
    {
        printf("Error: unable to close file.\n");
        exit(1);
    }

    printf("writer exiting...\n");
    exit(0);
}

void initalizeWriters()
{
    for (int i = 0; i <NUM_OUTPUTS; i++)
    {
        int newPid = fork();  // fork new process

        if (newPid == 0)  // child process
        {
            printf("Writer created!\n");
            writer(i);
        }

        else  // parent process
        {
            printf("adding writer PID\n");
            writers[i] = newPid;
        }
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
    char logPath[];

    long long runtime = end - start;

    // get size for the full path name
    int pathLen = strlen(logDir) + strlen(argv[0]) + strlen(".log");
    char *logPath = malloc(sizeof(char) * pathLen);  // allocate space for file path
    sprintf(logPath, "%s%s.log", logDir, argv[0]);   // format and set file path

    printf("\nlogging info to '%s'\n\n", logName);

    // open log file
    if ( (logFile = fopen(logPath, "a")) == NULL )
    {
        printf("Error: unable to open file '%s'\n", logName);
        exit(1);
    }

    // write to log file
    fprintf(logFile, "[%d, %lld],\n", workers, runtime);

    // close log file
    if ( fclose(logFile) == EOF )
    {
        printf("Error: unable to close file '%s'\n", logName);
        exit(1);
    }

    free(logPath);
}
