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

Both *spin-lock* and *atomic-lock* make the same system calls and each the same number of times. This
can be shown by running the following, which shows that the system calls and the number of times they
are called is identical.

```shell
strace -o spin-lock.txt -f -c ./spin-lock
strace -o atomic-lock.txt -f -c ./atomic-lock
strace -o lock-really.txt -f -c ./lock-really
diff spin-lock.txt atomic-lock.txt
```

