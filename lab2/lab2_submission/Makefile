LDLIBS = -lpthread -lcurl
CFLAGS = -Wall -Werror -Wpedantic -Wextra

all: readpe

readpe: readpe.c
	clang $(CFLAGS) readpe.c -o readpe

clean:
	rm -f readpe
