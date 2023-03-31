# COMP 3430 - Lab 5

Kameron Ronald  
ronaldk1@myumanitoba.ca  
7880495  

```shell
make        # compile mmap.c
make clean  # remonce mmap

./mmap mmap.txt
```

## Lab Report

For consistency, I will refer to the arguments of mmap() and munmap() based on the function prototypes given in MMAP(2) of the Linux Man Pages, which are as follows:

```c
void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);

int munmap(void *addr, size_t length);
```

**1. The first parameter to mmap() allows the caller to specify the starting address of the mapped segment, but it is generally not used (i.e. we usually let the OS pick the address). Why do you suppose this is generally not used? How do we ask the OS to pick the address?**

By passing a 0 (NULL) value to the argument _*addr_, mmap() lets the OS determine where to pick the starting address of the mapped segement. Generally this is the prefered method because it allows the OS to pick a location that does not conflict with any other existing mappings. Manually specifying the address could pontentially lead to problems, especially if the flag MAP_FIXED is used as well. As the Man Page describes:

> the use of MAP_FIXED is hazardous because it forcibly removes pre-existing mappings, making it easy for a multithreaded process to corrupt its own address space.

If not careful, specifying the _*addr_ could lead to unexpected consequences. Even when not using the MAP_FIX flag, if the given address conflicts with another mapping the OS will move it elsewhere, avoiding serious problems but still leading to unexpected results. Therefore,  letting the OS decide where to map the segment is the safest and most portable (consistent across different hardware) way to create a new mapping, and thus is most generally used.

**2. The fourth parameter to mmap() specifies the type of the mapped object. If the process wishes to modify the mapped memory so that other processes that map the same file can see the changes, which type should be used?**

The _flags_ agrument determines the behaviour of the mapping and can be 1 of the 3 types: MAP_SHARED, MAP_SHARED_VALIDATE, or MAP_PRIVATE. In order for other processes to see changes made to the file, either MAP_SHARED or MAP_SHARED_VALIDATE should be used. MAP_SHARED_VALIDATE checks that any other passed _flags_ are valid (there are a number of other optional flags), whereas MAP_SHARED does not.

**3. If a process that has a mapped memory segment executes a fork(), does the child process have access to the segment as well?**

Directly from the man pages:

> Memory mapped by mmap() is preserved across fork(2), with the same attributes.

So yes, a child process will have all the same access to the mapped memory segment as the parent did.

**4. Download and compile the program mmap.c. Prepare a short text file and call it “mmap.txt”. Run the program, and answer the following questions:**

**i. The system() function executes a shell command within a program. pmap command is being executed in mmap.c? Briefly describe this command.**



**ii. Examine the output of the program and find the listing for your mapped file. What is the size of the mapping?**



**iii. The program output should contain at least 2 listings for the compiled program (a.out). What is different about these listings? Can you explain this difference? (Hint - what does a process image look like?)**



**iv. Modify mmap.c. Immediately after the system() function call, print the value of the pointer returned by the mmap() function (use %p to print a pointer in hexadecimal). Does the return value of mmap and pmap match?**


