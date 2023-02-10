#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORKERS 100


int run_herder = 1;

char config[] = "./config.txt";  // config file name
int workers = 0;  // number of worker processes
int worker_ids[MAX_WORKERS] = {0};

void worker_handle_int() {
	printf("exiting worker (pid: %d)\n", getpid());
	exit(0);
}

static int read_config() {

	FILE* f_ptr;  // file pointer
	int num_workers = 0;  // new number of workers

    f_ptr = fopen(config, "r");
	if (f_ptr != NULL) {
		printf("reading config file...\n");
		fscanf(f_ptr, "%d", &num_workers);
	} else {
		printf("Error: unable to open config file.\n");
	}

	fclose(f_ptr);  // close file
	printf("workers: %d\n\n", num_workers);
	return num_workers;
}

static void update_workers(int num_workers) {
	printf("updating workers...\n");
	while (workers != num_workers && workers < MAX_WORKERS && workers >= 0) {
		if (workers < num_workers) {
			int new_pid = fork();
			if (new_pid == 0) {
				// child process
				printf("worker (pid: %d)\n", getpid());
				signal(SIGINT, worker_handle_int);
				while (1) ;  // just wait for the process herder to stop us
			} else {
				// parent process
				workers++;
				worker_ids[workers] = new_pid;
			}
		} else if (workers > num_workers) {
			// kill(worker_ids[workers], SIGINT);
			workers--;
		} else {
			printf("Umm... something ain't right\n");
		}
	}
	sleep(1);
	printf("\n");
}

void handle_hup() {
	update_workers(read_config());
}

void herder_handle_int() {
	printf("exiting...\n\n");
	update_workers(0);
	sleep(1);
	printf("End of process.\n\n");
	exit(0);
}

int main() {

	printf("============================\n");
	printf("Process Herder (pid: %d)\n", getpid());
	printf("============================\n\n");

	signal(SIGHUP, handle_hup);
	signal(SIGINT, herder_handle_int);

	update_workers(read_config());
	while (run_herder) ;  // infinite while loop until run_herder is set false

	printf("Process ended unexpectedly.\n");
	return 0;
}
