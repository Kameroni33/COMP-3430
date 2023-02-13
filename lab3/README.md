# ECE 3430 - Lab 3

Kameron Ronald  
ronaldk1@myumanitoba.ca  
7880495

## Compliation

```shell
make  # re-compile all files
```

## Part 1: Spin lock

```shell
./spin-lock
```

Unfortunately, this spin lock does not work (counter: 104717276). There exists a situation in which
the threads can be interupted during the execution of *mutex_lock* in a way such that they both set
the *lock* to be 1.

## Part 1: Atomic variables

```shell
./atomic-lock
```

**Hypothesis:** While variable operations (ie. changing the value of *lock*) will be handled
atomically, there still exists the possibility for one thread to be interupted after it has checked
*lock* and before it has set *lock* to 1. Thus, we would still have the same issue as with spin locks.
Therefore, I predict: "using atomic variables will **not** fix the lock".

**Result:** The atomic-lock performed much beter than the spin-lock and lock-maybe, but ultimately
it failed (counter: 196666944). Hence my hypothesis was correct.

## Part 1: Atomic instructions

```shell
./lock-really
```

This lock does work (counter: 200000000), because the entire operation to check and set the *lock*
is handled in a single atomic operation, rather than 2 seperate steps that could possubly be interupted.

## Part 1: Questions

Both *spin-lock* and *atomic-lock* make the same system calls. This can be seen by running `strace -c`
on both programs to get a summary of the number of system calls. Both yield the same result seen below:

```
% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
  0.00    0.000000           0         1           read
  0.00    0.000000           0         1           write
  0.00    0.000000           0         2           close
  0.00    0.000000           0        10           mmap
  0.00    0.000000           0         5           mprotect
  0.00    0.000000           0         1           munmap
  0.00    0.000000           0         3           brk
  0.00    0.000000           0         1           rt_sigaction
  0.00    0.000000           0         5           rt_sigprocmask
  0.00    0.000000           0         4           pread64
  0.00    0.000000           0         1         1 access
  0.00    0.000000           0         1           execve
  0.00    0.000000           0         2         1 arch_prctl
  0.00    0.000000           0         1           futex
  0.00    0.000000           0         1           set_tid_address
  0.00    0.000000           0         2           openat
  0.00    0.000000           0         3           newfstatat
  0.00    0.000000           0         1           set_robust_list
  0.00    0.000000           0         1           prlimit64
  0.00    0.000000           0         1           getrandom
  0.00    0.000000           0         1           rseq
  0.00    0.000000           0         2           clone3
------ ----------- ----------- --------- --------- ----------------
100.00    0.000000           0        50         2 total
```