CC       = clang
CFLAGS   = -Wall -Wpedantic -Wextra -Werror

OBJFILES = q1.o q2.o q3.o a2Utils.o
TARGETS  = q1 q2 q3

all: $(TARGETS)

q1: q1.o a2Utils.o
	$(CC) $(FLAGS) -o q1 q1.o a2Utils.o

q2: q2.o a2Utils.o
	$(CC) $(FLAGS) -o q2 q2.o a2Utils.o

q3: q3.o a2Utils.o
	$(CC) $(FLAGS) -o q3 q3.o a2Utils.o

clean:
	rm -f timeInMillis