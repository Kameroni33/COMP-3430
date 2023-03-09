CC      = clang
CFLAGS  = -Wall -Wpedantic -Wextra -Werror -g
TARGETS = serial withLocks withFIFOs
FOLDERS = output fifos

all: $(TARGETS) folders

# Create folders for output .txt's and FIFO's
folders:
	mkdir -p $(FOLDERS)

# Compile targets with a2Utils.c library
serial: serial.c a2Utils.c
	$(CC) $(FLAGS) -o serial serial.c a2Utils.c

withLocks: withLocks.c a2Utils.c
	$(CC) $(FLAGS) -o withLocks withLocks.c a2Utils.c

withFIFOs: withFIFOs.c a2Utils.c
	$(CC) $(FLAGS) -o withFIFOs withFIFOs.c a2Utils.c

clean:
	rm $(TARGETS)
	rm -rf $(FOLDERS)
