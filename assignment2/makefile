CC = clang
CFLAGS = -Wall -Wpedantic -Wextra -Werror

targets := question1 question2 question3

all: build

build: wordgetter timeInMillis

wordgetter: wordgetter.c
	$(CC) $(FLAGS) wordgetter.c -o wordgetter

timeInMillis: timeInMillis.c
	clang -Wall -Wpedantic -Wextra -Werror timeInMillis.c -o timeInMillis

clean:
	rm -f timeInMillis