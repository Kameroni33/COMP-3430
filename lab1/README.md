# COMP 3430: Lab 1

## Compile Instructions

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

**5.**