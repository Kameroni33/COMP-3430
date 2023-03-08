CC      = clang
CFLAGS  = -Wall -Wpedantic -Wextra -Werror
OBJECTS = question1.o question2.o question3.o a2Utils.o
TARGETS = question1 question2 question3

all: $(OBJECTS) $(TARGETS)

# Object files compiled via Implicit Rules
question1.o: question1.c
question2.o: question2.c
question3.o: question3.c
a2Utils.o: a2Utils.c

# Program files comipled from Object files ('linking' the utils library)
question1: question1.o a2Utils.o
	$(CC) $(FLAGS) -o question1 question1.o a2Utils.o

question2: question2.o a2Utils.o
	$(CC) $(FLAGS) -o question2 question2.o a2Utils.o

question3: question3.o a2Utils.o
	$(CC) $(FLAGS) -o question3 question3.o a2Utils.o

clean:
	rm -f *.o $(TARGETS)
