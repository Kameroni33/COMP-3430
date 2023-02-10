#include <stdio.h>



int start_thread() {
    printf("lol\n");
    return 1;
}


int main(int argc, char** argv) {

    // file variables
	char* f_name;
	FILE* f_ptr;

    // number of worker threads
    int workers = 0;

    if (argc > 1) {
		// read cofig file from the command line if provided
		f_name = argv[1];
	} else {
		// if no config file is provided use default (ie. 'config')
		f_name = argv[0];
	}

    // open file and read number of workers to start
    f_ptr = fopen(f_name, "r");
	if (f_ptr == NULL) {
		printf("Error: unable to open file '%s'.\n\n", f_name);
		exit(0);
	}

    fscanf(f_ptr, "%d");

    // start worker threads
    for (int i = 0; i < workers; i++) {
        assert(start_proc());
    }

	// close file
	fclose(f_ptr);

	printf("End of Processing.");
	return 0;
}