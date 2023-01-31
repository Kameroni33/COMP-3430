# COMP 3430: Lab 2

Kameron Ronald  
ronaldk1@myumanitoba.ca  
7880495


## Build Directions

`make` can be used to compile the following files. `make clean` can be used to delete any compiled files.

```
├── process.c
├── readpe.c
└── thread.c
```

For **part 1**, the executable `readpe` takes a single command line argument (the path to a file you would like to
parse). For example:

```shell
./readpe ./putty_executables/putty64x86.exe
```

3 binary PE files have been included for convenience: *putty64x86.exe*, *putty64xARM.exe*, and *putty32x86.exe* which
can be found in the folder *putty_executables* (downloaded from [here](https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html)).

For **part 2**, the executables `process` and `thread` have been icluded which simply start a new process/thread and
print "Hello World". Answers to the lab questions can be found below.


## Lab Questions


