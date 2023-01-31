#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define PE_SIGNATURE_INT 17744  // "PE\0\0" -> int

struct coff_header_t {
    uint16_t machine;
    uint16_t num_sections;
    uint32_t time_date_stamp;
    uint32_t symbol_table_ptr;
    uint32_t num_symbols;
    uint16_t optional_header_size;
    uint16_t characteristics;
};

int main(int argc, char* argv[]) {
    // check that we have exactly 1 parameter passed into the program call
    if (argc != 2) {
        printf("Error: Incorrect number of arguments (%d). Please specify file to open (ex. './readpe <file.name>').\n", argc);
        return 0;
    }

    // open file for processing
    unsigned int file_ptr = open(argv[1], O_RDONLY);
    if (file_ptr < 0) {
        printf("Error: Unable to open file '%s'.\n", argv[1]);
        return 0;
    }

    // read file signature and verify it's an PE format image file
    uint8_t sig_offset;
    uint32_t pe_signature;
    lseek(file_ptr, 0x3c, SEEK_SET);  // read signature offset at 0x3c (per documentation)
    read(file_ptr, &sig_offset, 1);
    lseek(file_ptr, sig_offset, SEEK_SET);  // read signature at offset sig_offset
    read(file_ptr, &pe_signature, 4);

    if (pe_signature != PE_SIGNATURE_INT) {
        printf("Error: Not a PE format image file.\n");
        return 0;
    }

    // read COFF header from file into struct
    struct coff_header_t coff = {0};
    read(file_ptr, &coff, 20);

    // convert time_date_stamp into type time_t for later display
    time_t datetime = (time_t)coff.time_date_stamp;

    // print out file information
    printf("File: %s\n\n"
           "Machine type: 0x%04x\n"
           "Number of sections: %d\n"
           "Created: %s"
           "Symbol table start: 0x%08x (should be 0)\n"
           "Number of symbols: %d (should be 0)\n"
           "Size of optional header: %d\n"
           "Characteristics: 0x%04x\n",
           argv[1], coff.machine, coff.num_sections, asctime(gmtime(&datetime)), coff.symbol_table_ptr, coff.num_symbols,
           coff.optional_header_size, coff.characteristics);

    printf("\nend of process.\n");
    return 0;
}
