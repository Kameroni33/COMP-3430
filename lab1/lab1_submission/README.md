# COMP 3430: Lab 1

Kameron Ronald  
7880495

## Compile Instructions

Assuming you are in the directory containing the `Makefile`, `os_info.c`, and `broken.c` files, all you need to do is
run:
```shell
make
```

## Questions

**1. What is the state of your shell?**

The current state is 'sleeping'.

**2. Why do you think your shell is in this state?**

The shell is waiting to read user input (to the stdin), which is a blocking function. Hence, the process is sleeping so
that other processes can use the CPU while it waits.

**3. What is the name of the currently running command? Is it your shell? Why isn’t it your shell?**

The current running process is *less* (not the shell) rather than a perhaps expected *tcsh*. This is because we ran the
*less* command in order to view the contents of `/proc/self/status` so it is actually the *less* process we are
currently in.

**4. What state does the currently running command have? Is this what you expected it to be?**

The state of our current process (ie. *less*) is currently 'running'. With question 3 in mind, this makes sense as we
are currently running the process in order to view the information. However, I do find it slightly odd that the *less*
process is not sleeping while it waits for further user input (as with our shell).

**5. Explain why the program could compile, but not successfully run, and what you did to make it go.**

There were 2 major issues with the program broken.c:

1. The array of pointers, *data*, was never initialized, and thus each element of *data* contained an address to some
random piece of memory. When the processor then tried to access the memory of these pointers, it is quite likely that it
tried to access memory that it didn't have permission to (and thus crashed with a segmentation fault). However, because
c lets to declare a variable without initializing it the compiler did not throw any warnings.
2. The sprintf() function call had its parameters in the incorrect order. It's syntax should be `int sprintf(char *str,
const char *string,...)`. However, because the output string (`data[i]->name`) and format string (`"%d %s\n"`) are both
of type char* the compiler did not throw any errors.

In order to fix these issues, I implemented the following changes:

* While not strictly necessary, I moved `#define DATA_LENGTH 10` to line 4 in order to follow common convention.
* Using malloc() I allocated memory space for each of *Data* structs as well as space for each of the *name* strings
held by the *Data* struct (lines 18 and 20). Additionally, I assigned the address values returned by malloc() to the
pointers in *data* and *data[i]->name* so that they pointed to actual data in memory. This addresses issue (1).
* I rearranged the order of parameters to the sprintf() call on line 21. Additionally, when allocating space for this
string in memory (line 20) I specified a character string of size 7 because I knew that each string created would be of
the form "i Hello" where i is the value 0-9. Hence, all strings generated should be of size 7 (plus an EOF character 
which does not contribute to the array's size in this case). This addresses issue (2).

Thus, broken.c can now compile and run without any errors. I'm still not sure what it's supposed to be doing, but
regardless it can now do it worry-free and happy.

**Note:** I have not added usages of free() to clean up memory allocation of the program. By default, the OS should
clean up any memory on termination of this process. That said, it is a good practice to implement free() calls in order
to explicitly clean up variables and avoid any potential memory leaks. In this situation, I have just implemented the
minimum requirements in order to get this program running bug-free and thus have not used any calls to free(). But in
most other cases, one should not fully rely on the OS to clean up memory. 

Expected Output:
```shell
8 Hello

```
