#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


enum e_ident_pos {
	EI_MAG0,
	EI_MAG1,
	EI_MAG2,
	EI_MAG3,
	EI_CLASS,
	EI_DATA,
	EI_VERSION,
	EI_OSABI,
	EI_ABIVERSION,
	EI_PAD0,
	EI_PAD1,
	EI_PAD2,
	EI_PAD3,
	EI_PAD4,
	EI_PAD5,
	EI_PAD6
};

struct elf_header_t {
	uint8_t e_ident[16];
	uint16_t e_type;
	uint16_t e_machine;
	uint32_t e_version;
	uint64_t e_entry;
	uint64_t e_phoff;
	uint64_t e_shoff;
	uint32_t e_flags;
	uint16_t e_ehsize;
	uint16_t e_phentsize;
	uint16_t e_phnum;
	uint16_t e_shentsize;
	uint16_t e_shnum;
	uint16_t e_shstrndx;
};


int main(int argc, char** argv) {

	// file variables
	char* f_name;
	FILE* f_ptr;

	// file header variables
	struct elf_header_t f_header;
	char endian[10];  // need at least 7 characters for 'endian\0'
	uint8_t bit;  // integer should be 32 or 64, so 8 bits is fine

	// file section header variables
	// ...


	if (argc > 1) {
		// read file name from the command line if provided
		f_name = argv[1];
	} else {
		// if no file name is provided, use this file as the default (ie. "a1q1")
		f_name = argv[0];
	}

	// open file
	f_ptr = fopen(f_name, "r");
	if (f_ptr == NULL) {
		printf("Error: unable to open file '%s'.\n\n", f_name);
		exit(0);
	}

	// read header data into struct
	fread(&f_header, sizeof(char), sizeof(f_header), f_ptr);

	// Verify it's an ELF file



	// assert that file is 64-bit
	if (f_header.e_ident[EI_CLASS] != 2) {
		printf("\nError: ELF file must be 64-bit.\n");
		exit(0);
	} else {
		bit = 64;  // for representation later
	}

	// determine endian
	if (f_header.e_ident[EI_DATA] == 1) {
		strcpy(endian, "little");
	} else if (f_header.e_ident[EI_DATA] == 2) {
		strcpy(endian, "big");
	} else {
		printf("\nError: unknown endian.\n");
		exit(0);
	}


	printf("ELF Header:\n");
	printf(" * %d-bit\n", bit);
	printf(" * %s endian\n", endian);
	printf(" * compiled for %#04x (operating system)\n", f_header.e_ident[EI_OSABI]);
	printf(" * object type is %#04x\n", f_header.e_type);
	printf(" * compiled for %#04x (isa)\n", f_header.e_machine);
	printf(" * entry point address is %#018lx\n", f_header.e_entry);
	printf(" * program header table starts at %#018lx\n", f_header.e_phoff);
	printf(" * section header table starts at %#018lx\n", f_header.e_shoff);
	printf(" * there are %d program headers, each is %d bytes\n", f_header.e_phnum, f_header.e_phentsize);
	printf(" * there are %d section headers, each is %d bytes\n", f_header.e_shnum, f_header.e_shentsize);
	printf(" * the section header string table is entry %d\n\n", f_header.e_shstrndx);

	// // byte 0x05 determines endianness
	// char f_endian = fgetc(f_ptr);
	// char f_endianstr[8];
    //     if (f_endian == 1) {
    //             strcpy(f_endianstr, "little");
    //     } else if (f_endian == 2) {
    //             strcpy(f_endianstr, "big");
    //     } else {
    //             strcpy(f_endianstr, "unknown");
    //     }

	// // byte 0x07 determines operating system ABI
	// fseek(f_ptr, 1, SEEK_CUR);
	// char f_abi = fgetc(f_ptr);

	// // bytes 0x10 and 0x11 determine object file type
	// fseek(f_ptr, 8, SEEK_CUR);
	// short* f_objtype = malloc(sizeof(short));
	// fread(f_objtype, 2, 1, f_ptr);
	
	// // bytes 0x12 and 0x13 determine instruction set architecture (isa)
	// short* f_isa = malloc(sizeof(short));
	// fread(f_isa, 2, 1,f_ptr);

	// // bytes 0x18 to 0x1F hold the memory address of the entry point
	// fseek(f_ptr, 4, SEEK_CUR);
	// long* f_entrypoint = malloc(sizeof(long));
	// fread(f_entrypoint, 8, 1, f_ptr);

	// // bytes 0x20 to 0x27 are the memory address of the program header table
	// long* f_programheader = malloc(sizeof(long));
	// fread(f_programheader, 8, 1, f_ptr);

	// // bytes 0x28 to 0x2F are memory address of the section header table
	// long* f_sectionheader = malloc(sizeof(long));
	// fread(f_sectionheader, 8, 1, f_ptr);

	// // bytes 0x36 and 0x37 contain the size of a program header table entry
	// fseek(f_ptr, 6, SEEK_CUR);
	// short* f_programheader_size = malloc(sizeof(short));
	// fread(f_programheader_size, 2, 1, f_ptr);

	// // bytes 0x38 and 0x39 contain the number of entries in the program header table
	// short* f_programheader_num = malloc(sizeof(short));
	// fread(f_programheader_num, 2, 1, f_ptr);

	// // bytes 0x3A and 0x3B contain the size of a section header table entry
	// short* f_sectionheader_size = malloc(sizeof(short));
	// fread(f_sectionheader_size, 2, 1, f_ptr);

	// // bytes 0x3C and 0x3D conatin the number of entries in the section header table
	// short* f_sectionheader_num = malloc(sizeof(short));
	// fread(f_sectionheader_num, 2, 1, f_ptr);

	// // bytes 0x3E and 0X3F contain the index of the string table
	// short* f_stringtable = malloc(sizeof(short));
	// fread(f_stringtable, 2, 1, f_ptr);

	// printf("ELF Header:\n"
	// 	" * 64-bit\n"  // since we have not exited execution, we can assume the file is 64-bit
	// 	" * %s endian\n"
	// 	" * compiled for 0x%02d (operating system)\n"
	// 	" * object type is 0x%02d\n"
	// 	" * compiled for 0x%02d (isa)\n"
	// 	" * entry point address is 0x%016lu\n"
	// 	" * program header table starts at 0x%016lu\n"
	// 	" * section header table starts at 0x%016lu\n"
	// 	" * there are %d program headers, each is %d bytes\n"
	// 	" * there are %d section headers, each is %d bytes\n"
	// 	" * the section header string table is entry %d\n\n",
	// 	f_endianstr, f_abi, *f_objtype, *f_isa, *f_entrypoint, *f_programheader, *f_sectionheader,
	// 	*f_programheader_num, *f_programheader_size, *f_sectionheader_num, *f_sectionheader_size,
	// 	*f_stringtable);

	// // print out information about each program header
	// for (int i = 0; i < *f_programheader_num; i++) {
		
	// 	// bytes 0x00 to 0x03 identifies the type of segment
	// 	fseek(f_ptr, (*f_programheader + (*f_programheader_size * i)), SEEK_SET);
	// 	unsigned int* segmentType = malloc(sizeof(int));
	// 	fread(segmentType, 4, 1, f_ptr);

	// 	// bytes 0x08 to 0x0F is the offest of the segment in memory
	// 	fseek(f_ptr, 4, SEEK_CUR);
	// 	unsigned long* segOffset = malloc(sizeof(long));
	// 	fread(segOffset, 8, 1, f_ptr);

	// 	// bytes 0x10 to 0x17 hold the virtual address of the segment in memory
	// 	unsigned long* virtualAddress = malloc(sizeof(long));
	// 	fread(virtualAddress, 8, 1, f_ptr);

	// 	// bytes 0x20 to 0x28 is the size of the segement in the file image
	// 	fseek(f_ptr, 8, SEEK_CUR);
	// 	unsigned long* sizeOnFileImage = malloc(sizeof(long));
	// 	fread(sizeOnFileImage, 8, 1, f_ptr);

	// 	int displayNumBytes = 32;
	// 	if (*sizeOnFileImage < 32) {
	// 		displayNumBytes = *sizeOnFileImage;
	// 	}

	// 	printf("Program header #%d:\n"
	// 		" * segment type 0x%08x\n"
	// 		" * virtual address of segment 0x%016lx\n"
	// 		" * size in file %lu bytes\n"
	// 		" * first %d bytes starting at file offset 0x%016lx:\n",
	// 		i, *segmentType, *virtualAddress, *sizeOnFileImage, displayNumBytes, *segOffset);
		
	// 	// print first (up to) 32 bytes of file
	// 	fseek(f_ptr, *segOffset, SEEK_SET);
	// 	unsigned char segByte;
	// 	for (int i = 0; i <displayNumBytes; i++) {
	// 		segByte = fgetc(f_ptr);
	// 		printf("%02x ", segByte);
	// 		if (i == 15) {
	// 			printf("\n");
	// 		}
	// 	}

	// 	printf("\n\n");

	// 	// free allocated memory
	// 	free(segmentType);
	// 	free(segOffset);
	// 	free(virtualAddress);
	// 	free(sizeOnFileImage);
	// }


	// close file
	fclose(f_ptr);

	// free alloced memory
	

	return 0;
}
