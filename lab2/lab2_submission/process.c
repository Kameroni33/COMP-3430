#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


int main() {
    printf("pid: %d\n", getpid());
    printf("creating new process...\n");
    // create a new process
    if (fork() == 0) {
        // child process
        printf("Child (%d)\n", getpid());
        printf("Hello World\n");
    } else {
        // parent process
        printf("Parent (%d)\n", getpid());
    }
    return 0;
}
