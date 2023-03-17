CFLAGS = -Wall -Wextra -Wpedantic -Werror -g
CC = clang

all: fsck-fat

fsck-fat: fsck-fat.c
	$(CC) $(CFLAGS) fsck-fat.c -o fsck-fat -include fat32.h

clean:
	rm -f fsck-fat
