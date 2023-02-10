//-------------------------------------------------------------------------------------------------
// NAME: Kameron Ronald
// STUDENT NUMBER: 7880495
// COURSE: COMP 3430
// INSTRUCTOR: Rob G.
// ASSIGNMENT: assignment 1, QUESTION: question 2a
// 
// REMARKS: this file reads a config file and spawns worker processes. Can be signaled with -HUP to
//			update the number of worker processes or -INT to clean up and exit.
//-------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdbool.h>

#define MAX_WORKERS 100  // Assumption: config file will contain a value bigger than 100


// GLOABL VARIABLES

bool run_herder = true;				// condition that process herder is running

char config[] = "./config.txt";		// config file name
int workers = 0;					// number of worker processes (only accessed by main process to avoid race conditions)
int worker_ids[MAX_WORKERS] = {0};	// pid's of all the worker processes

//-------------------------------------------------------------------------------------------------
// worker_exit
//
// PURPOSE: signal handler to stop a running process.
//-------------------------------------------------------------------------------------------------
void worker_exit() {
	printf("exiting worker (pid: %d)\n", getpid());
	exit(0);
}

//-------------------------------------------------------------------------------------------------
// read_config
//
// PURPOSE: read config file to determine number of workers to create.
// OUTPUT:	(int) number of workers to create.
//-------------------------------------------------------------------------------------------------
static int read_config() {

	FILE* f_ptr;			// file pointer
	int num_workers = 0;	// new number of workers

    f_ptr = fopen(config, "r");
	if (f_ptr != NULL) {
		printf("reading config file...\n");
		fscanf(f_ptr, "%d", &num_workers);
	} else {
		printf("Error: unable to open config file.\n");
	}

	fclose(f_ptr);			// close file
	printf("workers: %d\n\n", num_workers);
	return num_workers;
}

//-------------------------------------------------------------------------------------------------
// update_workers
//
// PURPOSE: update the number of worker processes running by creating new ones or signaling
//			existing ones to exit.
// INPUT:	(int) "num_workers" desired number of worker.
//-------------------------------------------------------------------------------------------------
static void update_workers(int num_workers) {

	int status;  // status code for exiting child processes

	printf("updating workers...\n");
	while (workers != num_workers && workers < MAX_WORKERS && workers >= 0) {

		// need to create new workers
		if (workers < num_workers) {

			int new_pid = fork();  // fork new process

			// child process
			if (new_pid == 0) {
				signal(SIGINT, worker_exit);  // link -INT to worker_exit handler

				// NOTE: using ctrl+C will signal workers to exit *twice*. This is completely safe, but to allow the herder process
				// to handle it per requirements use `kill -INT <pid-of-herder>` from a seperate termianl window.

				printf("worker (pid: %d)\n", getpid());
				while (1) ;	// just wait for the process herder to stop us

				printf("Worker exited unexpectedly.\n");
				exit(0);  // should never reach this point
			}

			// parent process
			else {
				workers++;						// update number of workers
				worker_ids[workers] = new_pid;	// add pid to list of pids
			}
		}

		// need to stop some workers
		else if (workers > num_workers) {

            status = 1;  						// successful exit should return status 0
			kill(worker_ids[workers], SIGINT);  // signal process to exit
            
			while (status != 0) {
				// wait for porcess to change state and verify that it exited
                waitpid(worker_ids[workers], &status, 0);
            }
            workers--;							// update number of workers
		}
		
		else {
			// should never reach this point
			printf("\nUmm... something ain't right\n");
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

	// BEGIN EXECUTION

	printf("============================\n");
	printf("Process Herder (pid: %d)\n", getpid());
	printf("============================\n\n");

	signal(SIGHUP, herder_update);	// link -HUP to herder_update handler
	signal(SIGINT, herder_exit);	// link -INT to herder_exit handler

	update_workers(read_config());	// do an initial read of the config file and create workers
	while (run_herder) ;			// infinite while loop until run_herder is set false

	printf("end of process.\n\n");
	return 0;
}
