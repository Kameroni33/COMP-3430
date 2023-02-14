LDLIBS = -lpthread
CFLAGS = -Wall -Wextra -Wpedantic -Werror
CC = clang

.PHONY: clean

all: lock-maybe spin-lock atomic-lock lock-really mutex-lock

trace:
	strace -o spin-lock.txt -f -c ./spin-lock
	strace -o atomic-lock.txt -f -c ./atomic-lock
	strace -o lock-really.txt -f -c ./lock-really
	strace -o mutex-lock.txt -f -c ./mutex-lock

clean:
	rm -f lock-maybe spin-lock atomic-lock lock-really mutex-lock
