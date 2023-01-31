LDLIBS = -lpthread -lcurl
CFLAGS = -Wall -Werror -Wpedantic -Wextra

all: readpe

readpe: readpe.c
	clang $(CFLAGS) read.pe -o readpe

clean:
	rm -f readpe
