LDLIBS = -lpthread
CFLAGS = -Wall -Wextra -Wpedantic -Werror
CC = clang

.PHONY: clean

all: lock-maybe spin-lock atomic-lock lock-really mutex-lock

clean:
	rm -f lock-maybe spin-lock atomic-lock lock-really mutex-lock
