#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAXNAME 100

// void link_process(int curr_cmd, int num_cmds, int pipes[][2], int argc[], char *args[][MAXARGS]);

int main(int argc, char *argv[]) {

    int pipes[2][2];  // track file pointers ('read'/'write') for both pipelines
    int pid;          // track current process ID

    char f_name[MAXNAME];

    if (argc > 1) {
        strcpy(f_name, argv[1]);
    } else {
        printf("Usage: './pipeline <file-name>'.\n Using default: 'README.md'\n");
        strcpy(f_name, "README.md");
    }

    char *args1[] = {"/bin/cat", f_name, NULL};  // process(1) arguments
    char *args2[] = {"/bin/tr", "'a-z'", "'A-Z'"};          // process(2) arguments
    char *args3[] = {"/bin/head", "-n", "5"};               // process(3) arguments

    // create a new pipeline
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
        close(pipes[0][0]);         // close pipeline(1) 'read' as we don't need it
        execv("/bin/cat", args1);   // execute process(3)
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
            close(pipes[0][1]);         // close pipeline(1) 'write' as we don't need it
            dup2(pipes[1][1], STDOUT_FILENO);  // link standard output to pipeline(2) 'write'
            close(pipes[1][0]);         // close pipeline(2) 'read' as we don't need it
            execv("/bin/tr", args2);    // execute process(2)
        }

        // Process 3 ==============================================================================
        else {
            dup2(pipes[1][0], STDIN_FILENO);   // link standard input to pipeline(2) 'read'
            close(pipes[1][1]);         // close pipeline(2) 'write' as we don't need it
            execv("/bin/head", args3);   // execute process(3)
        }
    }

}

// void link_process(int curr_cmd, int num_cmds, int pipes[][2], int argc[], char *args[][MAXARGS]) {

//     int pid;

//     // create a new pipeline
//     if (pipe(pipes[curr_cmd]) < 0) {
//         printf("pipe error\n\n");
//         exit(0);
//     }

//     // create new process
//     if ((pid = fork()) < 0) {
//         printf("fork error\n\n");
//         exit(0);
//     }

//     // parent Process
//     else if (pid > 0) {
//         dup2(pipes[curr_cmd][1], stdout);  // link standard output to 'write' of pipe
//         const int num_args = argc[curr_cmd];
//         char curr_args[num_args];
//         for (int i = 0; i < num_args; i++) {
//             // copy args for current command into new array
//             curr_args[i] = args[curr_cmd][i+1];
//         }
//         execv(args[curr_cmd][0], &curr_args);  // execute next command
//     }

//     // child Process
//     else {
//         if (curr_cmd < num_cmds) {
//             dup2(pipes[0][0], stdin);                               // link standard input to 'read' of pipe
//             link_process(++curr_cmd, num_cmds, pipes, argc, args);  // recursively link next command
//         } else {
//             return;  // we've reached the end of our chain of commands, so simply exit
//         }
//     }
// }
