CC      = clang
CFLAGS  = -Wall -Wpedantic -Wextra -Werror
OBJECTS = a2q1.o a2q2.o a2q3.o a2Utils.o
TARGETS = a2q1 a2q2 a2q3

all: $(OBJECTS) $(TARGETS)

# Object files compiled via Implicit Rules
a2q1.o: a2q1.c
a2q2.o: a2q2.c
a2q3.o: a2q3.c
a2Utils.o: a2Utils.c

# Program files comipled from Object files ('linking' the utils library)
a2q1: a2q1.o a2Utils.o
	$(CC) $(FLAGS) -o a2q1 a2q1.o a2Utils.o

a2q2: a2q2.o a2Utils.o
	$(CC) $(FLAGS) -o a2q2 a2q2.o a2Utils.o

a2q3: a2q3.o a2Utils.o
	$(CC) $(FLAGS) -o a2q3 a2q3.o a2Utils.o

clean:
	rm -f *.o $(TARGETS)
