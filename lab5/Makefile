CC = clang
CFLAGS = -Wall -Wextra -Wpedantic -Werror -g

all: mmap

mmap: mmap.c
	$(CC) $(CFLAGS) mmap.c -o mmap

clean:
	rm -f mmap
