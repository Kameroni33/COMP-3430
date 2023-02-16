#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<sys/wait.h>

int main() {

    int pipes[2][2];  // track file pointers ('read'/'write') for both pipelines
    int pid;          // track current process ID

    char *args1[] = {"/bin/cat", "blahhh.md", NULL};      // process(1) arguments
    char *args2[] = {"/bin/tr", "'a-z'", "'A-Z'", NULL};  // process(2) arguments
    char *args3[] = {"/bin/head", "-n", "5", NULL};       // process(3) arguments

    // create new process
    if ((pid = fork()) < 0) {
        printf("fork error\n\n");
        exit(0);
    }

    // Parent Process =============================================================================
    else if (pid > 0) {
        while(wait(NULL) > 0);  // wait for all child processes to terminate
    }
    
    else {

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

        // Child Process 1 ========================================================================
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

            // Child Process 2 ====================================================================
            else if (pid > 0) {
                dup2(pipes[0][0], STDIN_FILENO);   // link standard input to pipeline(1) 'read'
                close(pipes[0][1]);                // close pipeline(1) 'write' as we don't need it
                dup2(pipes[1][1], STDOUT_FILENO);  // link standard output to pipeline(2) 'write'
                close(pipes[1][0]);                // close pipeline(2) 'read' as we don't need it
                execv("/bin/tr", args2);           // execute process(2)
            }

            // Child Process 3 ====================================================================
            else {
                dup2(pipes[1][0], STDIN_FILENO);   // link standard input to pipeline(2) 'read'
                close(pipes[1][1]);                // close pipeline(2) 'write' as we don't need it
                execv("/bin/head", args3);         // execute process(3)
            }
        }
    }
}
