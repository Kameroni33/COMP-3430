#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>

#define PE_SIGNATURE 1346699264  // "PE\0\0"

//struct pe_coff_info {
//    uint16_t
//};

int main(int argc, char* argv[]) {
    // check that we have exactly 1 parameter passed into the program call
    if (argc != 2) {
        printf("Incorrect number of arguments (%d). Please specify file to open (ex. './readpe <file.name>').\n", argc);
        return 0;
    }

    // open file for processing
    pe_file = open(argv[1], O_RDONLY);
    if (pe_file < 0) {
        printf("Unable to open file '%s'.\n", argv[1]);
        return 0;
    }

    // read file signature and verify it's an PE format image file
    uint32_t pe_signature;
    lseek(pe_file, 0x3c, SEEK_SET);
    read(pe_file, &pe_signature);
    if (pe_signature != PE_SIGNATURE) {
        printf("Not a PE format image file.");
        return 0;
    }

    // read COFF header from file into struct


    // print out file information
    printf("File: %s\n\n"
           "Machine type: 0x____\n"
           "Number of sections: _\n"
           "Created: Sat Jul 10 09:51:55 2021\n"
           "Symbol table start: 0x________ (should be 0)\n"
           "Number of symbols: _ (should be 0)\n"
           "Size of optional header: ___\n"
           "Characteristics: 0x____\n",
           argv[1]);

    printf("\nend of process.\n");
    return 0;
}