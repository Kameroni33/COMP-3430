# COMP 3430 - Assignment 3

Kameron Ronald  
ronaldk1@myumanitoba.ca  
7880495

## Compiling and Running

```shell
make  # compile fat32.c file & create ./downloads folder
make clean  # remove compiled fat32 file adn ./downloads folder

./fat32  # see usages (more detailed)
./fat32 [imagefile] [command] {file}  # basic usage
```

For part 2, a number of file types (ex. hidden, system, etc..) have been excluded from the output. If these files are necessary the code is there, it's just been commented out to keep the output simplier (also I did not have time to implement long names).

For part 3, make will generate a ./downloads folder where files are copied to. File names use short names and are case sensative (typically all upper case).
