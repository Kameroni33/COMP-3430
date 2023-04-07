CC = clang
CFLAGS = -Wall -Wpedantic -Wextra -Werror -g

all: fat32 folders

fat32: fat32.c
	$(CC) $(CFLAGS) fat32.c -o fat32

folders:
	mkdir -p downloads

clean:
	rm -rf fat32 doenloads
