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

struct elf_file_header_t {
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

struct elf_program_header_t {
	uint32_t p_type;
	uint32_t p_flags;
	uint64_t p_offset;
	uint64_t p_vaddr;
	uint64_t p_paddr;
	uint64_t p_filesz;
	uint64_t p_memsz;
	uint64_t p_align;
};

struct elf_section_header_t {
	uint32_t sh_name;
	uint32_t sh_type;
	uint64_t sh_flags;
	uint64_t sh_addr;
	uint64_t sh_offset;
	uint64_t sh_size;
	uint32_t sh_link;
	uint32_t sh_info;
	uint64_t sh_addralign;
	uint64_t sh_entsize;
};


int main(int argc, char** argv) {

	// file variables
	char* f_name;
	FILE* f_ptr;

	// file header variables
	struct elf_file_header_t f_header;
	char endian[10];  // need at least 7 characters for 'endian\0'
	uint8_t bit;  // integer should be 32 or 64, so 8 bits is fine

	// file section header variables
	struct elf_program_header_t p_header;
	int display_num_bytes;
	unsigned char seg_byte;

	// file section header variables
	// struct elf_section_header_t s_header;


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

	// print out header information
	printf("ELF Header:\n");
	printf(" * %d-bit\n", bit);
	printf(" * %s endian\n", endian);
	printf(" * compiled for 0x%02x (operating system)\n", f_header.e_ident[EI_OSABI]);
	printf(" * object type is %#04x\n", f_header.e_type);
	printf(" * compiled for %#04x (isa)\n", f_header.e_machine);
	printf(" * entry point address is %#018lx\n", f_header.e_entry);
	printf(" * program header table starts at %#018lx\n", f_header.e_phoff);
	printf(" * section header table starts at %#018lx\n", f_header.e_shoff);
	printf(" * there are %d program headers, each is %d bytes\n", f_header.e_phnum, f_header.e_phentsize);
	printf(" * there are %d section headers, each is %d bytes\n", f_header.e_shnum, f_header.e_shentsize);
	printf(" * the section header string table is entry %d\n\n", f_header.e_shstrndx);

	for (int section = 0; section < f_header.e_phnum; section++) {

		// move to program header offset
		fseek(f_ptr, f_header.e_phoff + (section * f_header.e_phentsize), SEEK_SET);

		// read header data into struct
		fread(&p_header, sizeof(char), sizeof(f_header.e_phentsize), f_ptr);

		// limit number of bytes to print to at most 32 bytes
		if (p_header.p_filesz >= 32) {
			display_num_bytes = 32;
		} else {
			display_num_bytes = p_header.p_filesz;
		}

		// print program header information
		printf("Program header #%d:\n", section);
		printf(" * segment type %#010x\n", p_header.p_type);
		printf(" * virtual address of segment %#018lx\n", p_header.p_vaddr);
		printf(" * size in file %lu bytes\n", p_header.p_filesz);
		printf(" * first %d bytes starting at file offset %#018lx:\n", display_num_bytes, p_header.p_offset);
		
		// print first (up to) 32 bytes of file
		fseek(f_ptr, p_header.p_offset, SEEK_SET);
		for (int i = 0; i < display_num_bytes; i++) {
			seg_byte = fgetc(f_ptr);
			printf("%02x ", seg_byte);
			if (i == 15) {
				printf("\n");
			}
		}
		printf("\n\n");
	}


	// close file
	fclose(f_ptr);

	// free alloced memory
	

	return 0;
}
