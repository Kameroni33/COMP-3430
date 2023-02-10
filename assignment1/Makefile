CFLAGS = -Wall -Wextra -Wpedantic -Werror -g
LL = -lpthread
CC = clang

all: a1-elf a1-procs a1-threads a1-threads-locks

a1-elf: a1-elf.c
	$(CC) $(CFLAGS) a1-elf.c -o a1-elf

a1-procs: a1-procs.c
	$(CC) $(CFLAGS) a1-procs.c -o a1-procs

a1-threads: a1-threads.c
	$(CC) $(CFLAGS) a1-threads.c -o a1-threads $(LL)

a1-threads-locks: a1-threads-locks.c
	$(CC) $(CFLAGS) a1-threads-locks.c -o a1-threads-locks $(LL)

clean:
	rm -f a1-elf a1-procs a1-threads
