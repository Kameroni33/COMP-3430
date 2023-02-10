#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_WORKERS 100


int run_herder = false;
char config[] = "./config.txt";  // config file name

pthread_mutex_t signal_lock;
pthread_cond_t signal_available;
bool signal = false;

int workers = 0;  // number of worker processes
pthread_t worker_threads[MAX_WORKERS] = {0};


void* worker_thread(void* num) {
    
    int thread_num = num;
    printf("thread %d starting", thread_num);

    pthread_mutex_lock(&signal_lock);
    while(!signal) {
        pthread_cond_wait(&signal_available, &signal_lock);
    }
    signal = false;
    pthread_mutex_unlock(&signal_lock);

    printf("thread %d exiting", thread_num);
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
            worker_threads[workers] = new_thread;
            pthread_create(&new_thread, NULL, &worker_thread, workers);

            workers++;

		} else if (workers > num_workers) {

            workers--;

            pthread_mutex_lock(&signal_lock);
            signal = true;
            pthread_cond_broadcast(&signal_available);
            pthread_mutex_unlock(&signal_lock);

            // pthread_join(worker_threads[workers], NULL);  // don't know which thread will exit

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
	run_herder = 0;
}

int main() {

	printf("============================\n");
	printf("Thread Herder (pid: %d)\n", getpid());
	printf("============================\n\n");

	signal(SIGHUP, herder_update);
	signal(SIGINT, herder_exit);

    pthread_cond_init(&signal_available, NULL);
    pthread_mutex_init(&signal_lock, NULL);

	update_workers(read_config());
	while (run_herder) ;  // infinite while loop until run_herder is set false

	printf("end of process.\n\n");
	return 0;
}
