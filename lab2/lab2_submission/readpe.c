#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>

struct pe_signature {

};

int main(int argc, char* argv[]) {

    // check that we have exactly 1 parameter passed into the program call
    if (argc != 2) {
        printf("Incorrect number of arguments (%d). Please specify file to open (ex. './readpe <file.name>').\n", argc);
        return 0;
    }

    // open file for processing
    pefile = open(argv[1], O_RDONLY);
    if (pefile < 0) {
        printf("Unable to open file '%s'.\n", argv[1]);
        return 0;
    }

    // read file into pe_signature structure
    uint32_t pe_signature;
    read(handle, &pe_signature, 4);


    return 0;
}