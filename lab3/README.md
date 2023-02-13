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

Unfortunately, this spin lock does not work. There exists a situation in which the threads can be
interupted during the execution of *mutex_lock* in a way such that they both set the *lock* to be 1.

## Part 1: Atomic variables

**Hypothesis:** While variable operations (ie. changing the value of *lock*) will be handled
atomically, there still exists the possibility for one thread to be interupted after it has checked
*lock* and before it has set *lock* to 1. Thus, we would still have the same issue as with spin locks.
Therefore, I predict: "using atomic variables will **not** fix the lock".
