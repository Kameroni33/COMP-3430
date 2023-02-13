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

To answer this question, I used the following commands to compare the number of system calls each
process made. `-f` tracks child processes and `-c` formats the output to be easily comparable.

```shell
# direct strace output to file
strace -o spin-lock.txt -f -c ./spin-lock
strace -o atomic-lock.txt -f -c ./atomic-lock
strace -o lock-really.txt -f -c ./lock-really
# compare strace outputs
diff spin-lock.txt atomic-lock.txt
diff spin-lock.txt lock-really.txt
diff atomic-lock.txt lock-really.txt
```

All 3 processes: *spin-lock*, *atomic-lock*, and *lock-really* generally make the same system calls
the same number of times. Ocassionally I would see a different number of calls to `futex` depending on
the order threads executed/joined. Or, depending on how long threads were stuck waiting for locks the
number of microseconds per call would change. However, overall they generally had the same output
which can be seen below.

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
  0.00    0.000000           0         9           rt_sigprocmask
  0.00    0.000000           0         4           pread64
  0.00    0.000000           0         1         1 access
  0.00    0.000000           0         2           madvise
  0.00    0.000000           0         1           execve
  0.00    0.000000           0         2         1 arch_prctl
  0.00    0.000000           0         2           futex
  0.00    0.000000           0         1           set_tid_address
  0.00    0.000000           0         2           openat
  0.00    0.000000           0         3           newfstatat
  0.00    0.000000           0         3           set_robust_list
  0.00    0.000000           0         1           prlimit64
  0.00    0.000000           0         1           getrandom
  0.00    0.000000           0         3           rseq
  0.00    0.000000           0         2           clone3
------ ----------- ----------- --------- --------- ----------------
100.00    0.000000           0        61         2 total
```





