//-------------------------------------------------------------------------------------------------
// NAME: Kameron Ronald
// STUDENT NUMBER: 7880495
// COURSE: COMP 3430
// INSTRUCTOR: Rob G.
// ASSIGNMENT: assignment 1, QUESTION: question 2b
// 
// REMARKS: this file reads a config file and spawns worker threads. Can be signaled with -HUP to
//			update the number of worker threads or -INT to clean up and exit.
//-------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_WORKERS 100  // Assumption: config file will contain a value bigger than 100


// GLOABL VARIABLES

bool run_herder = true;                         // condition that process herder is running

char config[] = "./config.txt";                 // config file name
int workers = 0;                                // number of worker processes (only accessed by main thread to avoid race conditions)
bool worker_signals[MAX_WORKERS] = {0};         // signals per thread that specify if it should run or exit
pthread_t worker_threads[MAX_WORKERS] = {0};    // array of all worker threads

//-------------------------------------------------------------------------------------------------
// worker_thread
//
// PURPOSE: routine for new worker thread to run.
// INPUT:   (void*) "value" an int pointer to thread's 'id' number.
//-------------------------------------------------------------------------------------------------
void* worker_thread(void* value) {
    
    int* thread_id = value;                     // id of worker thread (pointer)
    bool* check = &worker_signals[*thread_id];  // check condition (pointer to array element with offset 'thread_id')
    printf("thread %d starting\n", *thread_id);

    while(!*check) ;  // wait for our stop to be set true

    printf("thread %d exiting\n", *thread_id);
    free(thread_id);  // thread_id was allocated in calling function so we need to free it
    pthread_exit(NULL);
}

//-------------------------------------------------------------------------------------------------
// create_thread
//
// PURPOSE: spawn a worker thread.
//-------------------------------------------------------------------------------------------------
static void create_thread() {

    pthread_t new_thread;

    int* curr_worker = malloc(sizeof(int));     // allocate memory space for thread_id (needs to be passed in as a pointer to thread function)
    *curr_worker = workers;                     // set worker number to current value of 'workers' ('workers' may change by the time our thread actually runs, so we need to save it's current value)
    worker_signals[workers] = false;            // signal that the new worker should not exit (careful to set before we start thread to avoid race condition)
    
    pthread_create(&new_thread, NULL, &worker_thread, curr_worker);
    
    worker_threads[workers] = new_thread;       // update our array of worker objects
    workers++;                                  // update number of workers
}

//-------------------------------------------------------------------------------------------------
// remove_thread
//
// PURPOSE: kill a worker thread.
//-------------------------------------------------------------------------------------------------
static void remove_thread() {
    workers--;                                      // update number of workers
    worker_signals[workers] = true;                 // signal that our worker can stop
    pthread_join(worker_threads[workers], NULL);    // wait for the worker to join (it will be a specific thread as opposed to just any one of our threads)
}

//-------------------------------------------------------------------------------------------------
// read_config
//
// PURPOSE: read config file to determine number of workers to create.
// OUTPUT:	(int) number of workers to create.
//-------------------------------------------------------------------------------------------------
static int read_config() {

	FILE* f_ptr;            // file pointer
	int num_workers = 0;    // new number of workers

    f_ptr = fopen(config, "r");
	if (f_ptr != NULL) {
		printf("reading config file...\n");
		fscanf(f_ptr, "%d", &num_workers);
	} else {
		printf("Error: unable to open config file.\n");
	}

	fclose(f_ptr);          // close file
	printf("workers: %d\n\n", num_workers);
	return num_workers;
}

//-------------------------------------------------------------------------------------------------
// update_workers
//
// PURPOSE: update the number of worker threads running by creating new ones or signaling
//			existing ones to exit.
// INPUT:	(int) "num_workers" desired number of worker.
//-------------------------------------------------------------------------------------------------
static void update_workers(int num_workers) {

	printf("updating workers...\n");
    while (workers != num_workers && workers < MAX_WORKERS && workers >= 0) {

        // need to create new workers
		if (workers < num_workers) {
			create_thread();
		}
        
        // need to stop some workers
        else if (workers > num_workers) {
            remove_thread();
		}
        
        else {
            // should never reach this point
			printf("Umm... something ain't right\n");
		}
    }

	sleep(1);
	printf("\n");
}

//-------------------------------------------------------------------------------------------------
// herder_update
//
// PURPOSE: signal handler to read config file and update number of workers.
//-------------------------------------------------------------------------------------------------
void herder_update() {
	update_workers(read_config());
}

//-------------------------------------------------------------------------------------------------
// herder_exit
//
// PURPOSE: signal handler to clean up all running workers and exit main herder process.
//-------------------------------------------------------------------------------------------------
void herder_exit() {
	printf("cleaning up\n\n");
	update_workers(0);
	run_herder = false;
}

//-------------------------------------------------------------------------------------------------
// main
//-------------------------------------------------------------------------------------------------
int main() {

	printf("============================\n");
	printf("Thread Herder (pid: %d)\n", getpid());
	printf("============================\n\n");

	signal(SIGHUP, herder_update);  // link -HUP to herder_update handler
	signal(SIGINT, herder_exit);    // link -INT to herder_exit handler

	update_workers(read_config());  // do an initial read of the config file and create workers
	while (run_herder) ;            // infinite while loop until run_herder is set false

	printf("end of process.\n\n");
	return 0;
}
