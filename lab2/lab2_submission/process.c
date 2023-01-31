#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


int main() {
    printf("pid: %d\n", getpid());
    printf("creating new process...\n\n");
    // create a new process
    int new_pid = fork();
    if (new_pid == 0) {
        // child process
        printf("child (pid: %d)\n", getpid());
        printf("Hello World\n");
    } else {
        // parent process
        printf("parent (pid: %d)\n", getpid());
        wait(&new_pid);
    }
    return 0;
}
