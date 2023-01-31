LDLIBS = -lpthread -lcurl
CFLAGS = -Wall -Werror -Wpedantic -Wextra

all: readpe process

readpe: readpe.c
	clang $(CFLAGS) readpe.c -o readpe

process: process.c
	clang $(CFLAGS) process.c -o process

clean:
	rm -f a.out readpe process thread
