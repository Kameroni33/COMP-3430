CFLAGS = -Wall -Wextra -Wpedantic -Werror -g
CC = clang

all: fsck-fat

fsck-fat: fsck-fat.c
	$(CC) $(CFLAGS) fsck-fat.c fat32.h -o fsck-fat

clean:
	rm -f fsck-fat
