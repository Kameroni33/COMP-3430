#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_WORKERS 100


bool run_herder = true;
char config[] = "./config.txt";  // config file name

int workers = 0;  // number of worker processes
pthread_t worker_threads[MAX_WORKERS] = {0};
bool worker_signals[MAX_WORKERS] = {0};


void* worker_thread(void* value) {
    
    int* thread_num = value;
    bool* check = &worker_signals[*thread_num];
    printf("thread %d starting\n", *thread_num);

    while(!*check) ;  // wait for our signal to to set true

    printf("thread %d exiting\n", *thread_num);
    pthread_exit(NULL);
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

			pthread_t new_thread;
            int curr_worker = workers;
            pthread_create(&new_thread, NULL, &worker_thread, &curr_worker);
            worker_threads[workers] = new_thread;
            worker_signals[workers] = false;
            workers++;

		} else if (workers > num_workers) {

			workers--;
            worker_signals[workers] = true;
            pthread_join(worker_threads[workers], NULL);

		} else {
			printf("Umm... something ain't right\n");
		}
    }

	sleep(1);
	printf("\n");
}

void herder_update() {
	update_workers(read_config());
}

void herder_exit() {
	printf("cleaning up\n\n");
	update_workers(0);
	run_herder = false;
}

int main() {

	printf("============================\n");
	printf("Thread Herder (pid: %d)\n", getpid());
	printf("============================\n\n");

	signal(SIGHUP, herder_update);
	signal(SIGINT, herder_exit);

	update_workers(read_config());
	while (run_herder) ;  // infinite while loop until run_herder is set false

	printf("end of process.\n\n");
	return 0;
}
