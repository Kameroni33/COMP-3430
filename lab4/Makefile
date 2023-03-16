CFLAGS = -Wall -Wextra -Wpedantic -Werror -g
CC     = clang

all: fsckMBR

fsckMBR: fsckMBR.c
	$(CC) $(CFLAGS) fsckMBR.c -o fsckMBR

fsckFAT: fsckFAT.c
	$(CC) $(CFLAGS) fsckFAT.c -o fsckFAT

clean:
	rm -f fsckMBR fsckFAT
