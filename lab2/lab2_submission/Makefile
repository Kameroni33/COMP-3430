LDLIBS = -lpthread
CFLAGS = -Wall -Werror -Wpedantic -Wextra

all: readpe process thread

readpe: readpe.c
	clang $(CFLAGS) readpe.c -o readpe

process: process.c
	clang $(CFLAGS) process.c -o process

thread: thread.c
	clang $(CFLAGS) thread.c -o thread $(LDLIBS)

clean:
	rm -f a.out readpe process thread
