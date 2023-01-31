#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


void* thread_task(void* ignored)
{
    printf("thread (pid: %d)", getpid());
    printf("Hello World\n");
    pthread_exit(0);
}


int main() {
    printf("pid: %d\n", getpid());
    printf("creating new process...\n\n");
    // create a new thread
    pthread_t new_thread;
    pthread_create( &new_thread, NULL, thread_task, NULL );
    pthread_join(new_thread, NULL);
    return 0;
}