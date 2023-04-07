CC      = clang
CFLAGS  = -Wall -Wpedantic -Wextra -Werror -g

all: fat32 output

fat32: fat32.c
	$(CC) $(CFLAGS) fat32.c -o fat32

output:
	mkdir -p output

clean:
	rm -rf fat32 output
