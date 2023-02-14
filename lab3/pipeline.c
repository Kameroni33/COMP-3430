#include <stdio.h>

#define MAXLINE 100
#define MAXARGS 10




void link_process(int curr_cmd, int num_cmds, int pipes[][2], int argc[], char *args[][MAXARGS]);


int main(int argc, char *argv[]) {

    // VARIABLES
    FILE *f_ptr;
    char *f_name;
    int pipes[3][2];
    int pid;
    char line[MAXLINE];

    int commands = 3;

    char *args1[][MAXARGS] = {{"/bin/cat", "hello-world.txt", NULL}, {"tr", "'a-z'", "'A-Z'"}, {"head", "-n", "5"}};

    link_process(0, 3);

}

void link_process(int curr_cmd, int num_cmds, int pipes[][2], int argc[], char *args[][MAXARGS]) {

    int pid;

    // create a new pipeline
    if (pipe(pipes[curr_cmd]) < 0) {
        printf("pipe error\n\n");
        exit(0);
    }

    // create new process
    if ((pid = fork()) < 0) {
        printf("fork error\n\n");
        exit(0);
    }

    // parent Process
    else if (pid > 0) {
        dup2(pipes[curr_cmd][1], stdout);  // link standard output to 'write' of pipe
        const int num_args = argc[curr_cmd];
        char curr_args[num_args];
        for (int i = 0; i < num_args; i++) {
            // copy args for current command into new array
            curr_args[i] = args[curr_cmd][i+1];
        }
        execv(args[curr_cmd][0], &curr_args);  // execute next command
    }

    // child Process
    else {
        if (curr_cmd < num_cmds) {
            dup2(pipes[0][0], stdin);                               // link standard input to 'read' of pipe
            link_process(++curr_cmd, num_cmds, pipes, argc, args);  // recursively link next command
        } else {
            return;  // we've reached the end of our chain of commands, so simply exit
        }
    }
}
