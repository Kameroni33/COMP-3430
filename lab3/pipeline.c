#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAXNAME 100   // provided file name should not exceed this length

int main(int argc, char *argv[]) {

    int pipes[2][2];  // track file pointers ('read'/'write') for both pipelines
    int pid;          // track current process ID

    char f_name[MAXNAME];

    if (argc > 1) {
        // read file path from command line argument if provided
        strcpy(f_name, argv[1]);
    } else {
        // if no file path was provided, use the default "README.md"
        printf("Usage: './pipeline <file-path>'.\n");
        strcpy(f_name, "README.md");
    }

    printf("File: '%s'.\n\n", f_name);

    char *args1[] = {"/bin/cat", "./test.md", NULL};           // process(1) arguments
    char *args2[] = {"/bin/tr", "'a-z'", "'A-Z'", NULL};  // process(2) arguments
    char *args3[] = {"/bin/head", "-n", "5", NULL};       // process(3) arguments

    // create pipeline
    if (pipe(pipes[0]) < 0) {
        printf("pipe error\n\n");
        exit(0);
    }

    // create new process
    if ((pid = fork()) < 0) {
        printf("fork error\n\n");
        exit(0);
    }

    // Process 1 ==================================================================================
    else if (pid > 0) {
        dup2(pipes[0][1], STDOUT_FILENO);  // link standard output to pipeline(1) 'write'
        close(pipes[0][0]);                // close pipeline(1) 'read' as we don't need it
        execv("/bin/cat", args1);          // execute process(1)
    }

    else {

        // create a new pipeline
        if (pipe(pipes[1]) < 0) {
            printf("pipe error\n\n");
            exit(0);
        }

        // create new process
        if ((pid = fork()) < 0) {
            printf("fork error\n\n");
            exit(0);
        }

        // Process 2 ==============================================================================
        else if (pid > 0) {
            dup2(pipes[0][0], STDIN_FILENO);   // link standard input to pipeline(1) 'read'
            close(pipes[0][1]);                // close pipeline(1) 'write' as we don't need it
            dup2(pipes[1][1], STDOUT_FILENO);  // link standard output to pipeline(2) 'write'
            close(pipes[1][0]);                // close pipeline(2) 'read' as we don't need it
            execv("/bin/tr", args2);           // execute process(2)
        }

        // Process 3 ==============================================================================
        else {
            dup2(pipes[1][0], STDIN_FILENO);   // link standard input to pipeline(2) 'read'
            close(pipes[1][1]);                // close pipeline(2) 'write' as we don't need it
            execv("/bin/head", args3);         // execute process(3)
        }
    }

}
