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

**Result:** The atomic-lock did not work, in fact it performed worse than both spin-lock and
lock-maybe (counter: 196666944). Hence my hypothesis was correct.

## Part 1: Atomic instructions

