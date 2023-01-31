LDLIBS = -lpthread -lcurl
CFLAGS = -Wall -Werror -Wpedantic -Wextra

all: download

readpe: readpe.c
	clang $(CFLAGS) read.pe -o readpe

clean:
	rm -f readpe
