#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define REFRESH_RATE 1


int main() {
	// create timespec struct for sleeping later
	struct timespec req, rem;
	req.tv_sec = 0;
	req.tv_nsec = 500000000L;
	// print out initial loading bar
	printf("loading... ( 000 )");
	printf("\033[2D");
	fflush(stdout);
	// iterate from 1 to 100 and print the loading progress
	for (int i = 0; i <= 100; i++) {
		// move cursor to correct location
		if (i < 10) {
			printf("\033[1D");
		} else if (i < 100) {
			printf("\033[2D");
		} else {
			printf("\033[3D");
		}
		// print next number
		printf("%d", i);
		fflush(stdout);
		// add a delay so that the loading bar isn't instant
		nanosleep(&req, &rem);
	}
	printf("\033[2C\n");
	printf("done.\n");
}
