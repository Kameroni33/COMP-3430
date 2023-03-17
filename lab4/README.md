# COMP 3430 - Lab 4

Kameron Ronald  
roanldk1@myumanitoba.ca  
7880495  

## Compile & Run

```shell
make clean  # removes compiled fsck-fat executable
make        # compiles fsck-fat.c with fat32.h

./fsck-fat <volume-file(s)>
```

For convience, `./fack-fat` accepts any number of volume-file names, and iterates through them all printing information about them. Thus, the following command is possible

```shell
./fsck-fat ~comp3430/shared/lab4_volumes/*.img
```
