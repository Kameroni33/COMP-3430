#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

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

// Global Constants ===================================================================

#define NUM_OUTPUTS 27  // number of output files (a-z + other)
#define MAX_WORD 100    // max character count for words in input files
#define MAX_NAME 25     // max characters for input file names

// Methods ============================================================================

void initializeOutputs(FILE *outputFiles[NUM_OUTPUTS], char *outputPaths[NUM_OUTPUTS])
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

void closeOutputs(FILE *outputFiles[NUM_OUTPUTS])
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

FILE* determineOutputFile(FILE *outputFiles[NUM_OUTPUTS], char word[MAX_WORD])
{
    // uppercase ASCII letters range from 65 - 90
    if ( word[0] > 64 && word[0] < 91 )
    {
        // return the correct output file descriptor by offsetting by 65
        return outputFiles[word[0]-65];
    }

    // lowercase ASCII letters range from 97 - 122
    else if ( word[0] > 96 && word[0] < 123 )
    {
        // return the correct output file descriptor by offsetting by 97
        return outputFiles[word[0]-97];
    }

    else
    {
        // all other characters go the same output file (last one in our list of files)
        return outputFiles[NUM_OUTPUTS-1];
    }
}

void processFile(char *inputPath, int threadSafe)
{
    FILE *inputFile;      // file descriptor for input file
    char word[MAX_WORD];  // current word in file

    if ((inputFile = fopen(inputPath, "r")) == NULL)  // open input file
    {
        printf("Error: unable to open file '%s'.\n", inputPath);
        exit(1);
    }

    // scan through entire input file
    while (fscanf(inputFile, "%s", word) != EOF)
    {
        if (threadSafe)
        {
            // determine output file

            // aquire lock for the file we want

            // append current word

            // release lock on output file
        }

        else
        {
            // determine output file and append current word directly (thread-unsafe)
            fprintf(determineOutputFile(outputFiles, word), "%s\n", word);
        }
        
    }

    if ( fclose(inputFile) == EOF )  // close current input file
    {
        printf("Error: unable to close file '%s'\n", inputPath);
        exit(1);
    }
}

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
    fprintf(logFile, "========================================\n");
    fprintf(logFile, "Program: %s\n", argv[0]+2);
    fprintf(logFile, "Files:   %d\n", argc);
    fprintf(logFile, "Workers: %d\n", workers);
    fprintf(logFile, "Start:   %lld ms\n", start);
    fprintf(logFile, "End:     %lld ms\n", start);
    fprintf(logFile, "Runtime: %lld ms\n", end - start);
    fprintf(logFile, "========================================\n\n");

    // close log file
    if ( fclose(logFile) == EOF )
    {
        printf("Error: unable to close file '%s'\n", logName);
        exit(1);
    }
}