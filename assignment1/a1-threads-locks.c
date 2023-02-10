#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_WORKERS 100


bool run_herder = true;
char config[] = "./config.txt";  // config file name

pthread_mutex_t signal_lock;
pthread_cond_t signal_cond;
bool thread_signal = false;

pthread_mutex_t workers_lock;
pthread_cond_t workers_cond;
bool workers_updated = false;

int workers = 0;  // number of worker processes
pthread_t worker_threads[MAX_WORKERS] = {0};


void* worker_thread() {
    
    pthread_mutex_lock(&workers_lock);
    printf("thread %d starting\n", workers);
    workers++;
    workers_updated = true;
    pthread_cond_signal(&workers_cond);
    pthread_mutex_unlock(&workers_lock);

    pthread_mutex_lock(&signal_lock);
    while(!thread_signal) {
        pthread_cond_wait(&signal_cond, &signal_lock);
    }
    thread_signal = false;
    pthread_mutex_unlock(&signal_lock);

    pthread_mutex_lock(&workers_lock);
    workers--;
    printf("thread %d exiting\n", workers);
    workers_updated = true;
    pthread_cond_signal(&workers_cond);
    pthread_mutex_unlock(&workers_lock);

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

    pthread_mutex_lock(&workers_lock);
    int to_create = num_workers - workers;
    pthread_mutex_unlock(&workers_lock);

    if (to_create > 0) {
        for (int i = 0; i < to_create; i++) {

            pthread_mutex_lock(&workers_lock);
            pthread_t new_thread;
            pthread_create(&new_thread, NULL, &worker_thread, NULL);
            worker_threads[workers] = new_thread;
            while(!workers_updated) {
                pthread_cond_wait(&workers_cond, &workers_lock);
            }
            workers_updated = false;
            pthread_mutex_unlock(&workers_lock);
        }
    } else if (to_create < 0) {
        for (int i = 0; i > to_create; i--) {

            pthread_mutex_lock(&signal_lock);
            thread_signal = true;
            pthread_cond_signal(&signal_cond);
            pthread_mutex_unlock(&signal_lock);

            pthread_mutex_lock(&workers_lock);
            while(!workers_updated) {
                pthread_cond_wait(&workers_cond, &workers_lock);
            }
            workers_updated = false;
            pthread_mutex_unlock(&workers_lock);
        }
    } else {
        printf("no changes required\n");
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

    pthread_cond_init(&signal_cond, NULL);
    pthread_cond_init(&workers_cond, NULL);
    pthread_mutex_init(&signal_lock, NULL);
    pthread_mutex_init(&workers_lock, NULL);

	update_workers(read_config());
	while (run_herder) ;  // infinite while loop until run_herder is set false

	printf("end of process.\n\n");
	return 0;
}
