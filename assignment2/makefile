CC      = clang
CFLAGS  = -Wall -Wpedantic -Wextra -Werror
TARGETS = q1 q2 q3

all: $(TARGETS)

q1: q1.o a2Utils.o
	$(CC) $(FLAGS) -o q1 q1.o a2Utils.o

q2: q2.o a2Utils.o
	$(CC) $(FLAGS) -o q2 q2.o a2Utils.o

q3: q3.o a2Utils.o
	$(CC) $(FLAGS) -o q3 q3.o a2Utils.o

clean:
	rm -f *.o 
