#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_WORKERS 100


int run_herder = 1;

char config[] = "./config.txt";  // config file name
int workers = 0;  // number of worker processes
int worker_ids[MAX_WORKERS] = {0};

void worker_exit() {
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
		// printf("workers: %d | num: %d\n", workers, num_workers);  // testing
		if (workers < num_workers) {

			int new_pid = fork();
			if (new_pid == 0) {

				// child process
				printf("worker (pid: %d)\n", getpid());
				signal(SIGINT, worker_exit);
				while (1) ;  // just wait for the process herder to stop us
				printf("Worker exited unexpectedly.\n");
				exit(0);

			} else {

				// parent process
				workers++;
				worker_ids[workers] = new_pid;

			}
		} else if (workers > num_workers) {

			// printf("signaling process (%d)\n", worker_ids[workers]);  // testing
            int status = 1;
			kill(worker_ids[workers], SIGINT);
            while (status != 0) {
                waitpid(worker_ids[workers], &status, 0);
            }
            printf("worker exited!");
            workers--;

		} else {
			printf("Umm... something ain't right\n");
		}
	}
	sleep(1);
	printf("\n");
}

void handle_update() {
	update_workers(read_config());
}

void herder_exit() {
	printf("cleaning up\n\n");
	update_workers(0);
	run_herder = 0;
}

int main() {

	printf("============================\n");
	printf("Process Herder (pid: %d)\n", getpid());
	printf("============================\n\n");

	signal(SIGHUP, handle_update);
	signal(SIGINT, herder_exit);

	update_workers(read_config());
	while (run_herder) ;  // infinite while loop until run_herder is set false

	printf("end of process.\n\n");
	return 0;
}
