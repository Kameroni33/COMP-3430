CC      = clang
CFLAGS  = -Wall -Wpedantic -Wextra -Werror
OBJECTS = q1.o q2.o q3.o a2Utils.o
TARGETS = q1 q2 q3

all: $(OBJECTS) $(TARGETS)

# Object files compiled via Implicit Rules
q1.o: q1.c
q2.o: q2.c
q3.o: q3.c
a2Utils.o: a2Utils.c

# Program files comipled from Object files ('linking' the utils library)
q1: q1.o a2Utils.o
	$(CC) $(FLAGS) -o q1 q1.o a2Utils.o

q2: q2.o a2Utils.o
	$(CC) $(FLAGS) -o q2 q2.o a2Utils.o

q3: q3.o a2Utils.o
	$(CC) $(FLAGS) -o q3 q3.o a2Utils.o

clean:
	rm -f *.o $(TARGETS)
