# COMP 3430: Lab 2

Kameron Ronald  
ronaldk1@myumanitoba.ca  
7880495


## Build Directions

`make` can be used to compile the following files. `make clean` can be used to delete all compiled files.

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

1. *What system calls have you written into your programs?*

For the first part of the lab, I used the system calls `open`, `read`, and `lseek`. In the second part, I used `getpid`
and `fork` (for processes) as well as `pthread_create`, `pthread_join` and `pthread_exit` (for threads). 

2. *What system calls does your program actually make to create a process or thread?*

Creating a new process calls the `clone` system call. Taken from `strace` we can see the actual call made:

```
clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD, child_tidptr=0x7f90e9687a10) = 242572
```

Creating a new thread executes a number of system calls, including: `rt_sigaction`, `rt_sigprocmask`, `mmap`,
`mprotect`, `clone3`, and `futex`. However, it seems `clone3` is the call to actually create the new thread while the
others deal with signals, memory space, etc. Taken from `strace` we can see the actual call made:

```
clone3({flags=CLONE_VM|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_THREAD|CLONE_SYSVSEM|CLONE_SETTLS|CLONE_PARENT_SETTID|CLONE_CHILD_CLEARTID, child_tid=0x7f6ec2b0d910, parent_tid=0x7f6ec2b0d910, exit_signal=0, stack=0x7f6ec230d000, stack_size=0x7fff00, tls=0x7f6ec2b0d640} => {parent_tid=[242762]}, 88) = 242762
```

3. *Are the calls made in the process version of the code different from the thread version of the code? How?*

blah

4. *Add code to wait for the child process, or join a thread. Are the syscalls for these commands different? Why do you
think they are the same or different?*

blah blah
