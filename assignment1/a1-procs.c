#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


static int start_proc() {
    printf("lol\n");
    return 1;
}


int main() {

    // file variables
	char f_name[] = "./config.txt";
	FILE* f_ptr;

    // number of worker processes
    int workers = 0;

	printf("Herder (pid: %d)", getpid());

    // open file and read number of workers to start
    f_ptr = fopen(f_name, "r");
	if (f_ptr != NULL) {
		printf("reading config file...\n");
		fscanf(f_ptr, "%d", &workers);
		printf("workers: %d\n", workers);
	} else {
		printf("Error: unable to open config file.\n\n");
		exit(0);
	}
    

    // start worker processes
    for (int i = 0; i < workers; i++) {
        start_proc();
    }

	// close file
	fclose(f_ptr);

	printf("End of Processing.\n");
	return 0;
}
