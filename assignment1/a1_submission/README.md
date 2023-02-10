# COMP 3430: Assignment 1

Kameron Ronald  
ronaldk1@myumanitoba.ca  
7880495  

## Compilation

A _Makefile_ has been included in the solution.

```shell
make            # compile all files
make clean      # remove all compiled files
```

## ELF reader

The process __a1-elf__ takes an optional command line argument to specify what ELF file you want to read. If no arguments are provided,
the process will use itself as the ELF file (ie. './a1-elf').

Usage:
```shell
./a1-elf <optional-file-path>
```

*Note: This process assumes that the section names will not exceed MAX_NAME characters, as defined by the process (by default is 100). Not*
*the most space efficient, I know, but it was a quick fix at the time and the deadline for this assignment is fast approaching.*

## Process Herder

The process __a1-procs__ takes no command line arguments. It does, however, require a configuration file _config.txt_. I've provided a
sample config file for convience.

To quickly change the config file value:
```shell
echo <value> > ./config.txt
```

To update the Process Herder it must be signaled. On start-up the process will print its PID.
```shell
kill -HUP <pid>     # signals the process to re-read the config file and update number of workers
kill -INT <pid>     # signals the process to safely clean up all workers and exit
```

## Thread Herder

The process __a1-threads__ takes no command line arguments. Again, it requires a configuration file same as the Process Herder. The
Thread Herder also handles the same signals as before, using -HUP to update the number of workers and -INT to safely clean up all
workers.

I've actually included 2 solutions for this part. __a1-threads__ uses a shared memory array to communicate between threads, using
pthread_join() to wait until a specific thread exits. __a1-threads-locks__ uses locks and condition variables to communicate between
threads and waits until any one thread exits. *__a1-threads__ is my main solution (please graded this one)*, I've just included the
other solution for fun (I actually made it first before I realized we weren't supposed to use locks, but figured I'd throw it in
anyways since I already did the work).

*Note: Both the Process Herder and Thread Herder make a couple of assumptions about the config file. In particular, that it contains*
*a single line with a single numeric value. It also assumes that the value is non-negative and not exceeding MAX_WORKERS, defined by*
*the process (by defualt is 100).*