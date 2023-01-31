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

For **part 2**, the executables `process` and `thread` have been included which simply start a new process/thread and
print "Hello World" (along with some pid information). Answers to the lab questions can be found below.


## Lab Questions

1. *What system calls have you written into your programs?*

For the first part of the lab, I used the system calls `open`, `read`, and `lseek`. In the second part, I used `getpid`;
`wait` and `fork` (for processes); and `pthread_create`, `pthread_exit` and `pthread_join` (for threads). 

2. *What system calls does your program actually make to create a process or thread?*

Creating a new process calls the `clone` system call. Taken from `strace` we can see the actual call made:

```
clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD, child_tidptr=0x7f90e9687a10) = 242572
```

Creating a new thread executes a number of system calls, including: `rt_sigaction`, `rt_sigprocmask`, `mmap`,
`mprotect`, and finally `clone3`. `clone3` is the call to actually create the new thread while the others deal with
signals, memory space, etc. Taken from `strace` we can see the actual call made:

```
clone3({flags=CLONE_VM|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_THREAD|CLONE_SYSVSEM|CLONE_SETTLS|CLONE_PARENT_SETTID|CLONE_CHILD_CLEARTID, child_tid=0x7f6ec2b0d910, parent_tid=0x7f6ec2b0d910, exit_signal=0, stack=0x7f6ec230d000, stack_size=0x7fff00, tls=0x7f6ec2b0d640} => {parent_tid=[242762]}, 88) = 242762
```

3. *Are the calls made in the process version of the code different from the thread version of the code? How?*

Other than the extra calls to deal with signals and memory space, creating a new process called `clone` while creating a
new thread called `clone3`. These 2 calls are nearly identical, both are used to create a child process. The main
difference is that `clone3` is a newer improved version of `clone`. From the linux man pages:

> "The **clone3**() system call provides a superset of the functionality of the older **clone**() interface. It also
provides a number of API improvements, including: space for additional flags bits; cleaner separation in the use of
various arguments; and the ability to specify the size of the child's stack area."

Note that because threads have a shared memory space, a number of extra system calls are made in order to set up the
environment before `clone3` is called.

4. *Add code to wait for the child process, or join a thread. Are the syscalls for these commands different? Why do you
think they are the same or different?*

When waiting for a child process the `wait4` system call is made. When waiting for a thread to join the `futex` system
call is made. I think the main reason for this difference is due to the shared memory space feature of threads. In order
for a process to know when a child process is finished it must wait and be notified by the os when the child process
changes states. However, because threads share a memory space, a thread that is waiting for another to join can just
monitor a variable in its shared memory and the os needs not get involved. Both are valid methods, but each is better
suited for the environment it is in. This is backed up my the man pages, which state the following about those 2 calls.

About `wait` and hence consequently in regard to `wait4`:
> "...these system calls are used to wait for state changes in a child of the calling process, and obtain information
about the child whose state has changed."

And regarding `futex`:
> "The **futex()** system call provides a method for waiting until a certain condition becomes true. It is typically
used as a blocking construct in the context of shared-memory synchronization. When using futexes, the majority of the
synchronization operations are performed in user space."
