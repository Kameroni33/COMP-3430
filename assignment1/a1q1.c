#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_NAME 100


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

	// file program/section header variables
	struct elf_program_header_t p_header;
	struct elf_section_header_t s_header;
	unsigned char seg_byte;
	int display_num_bytes;  // number of bytes to print out (at most 32)
	char s_name[MAX_NAME];

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
	if (f_header.e_ident[EI_MAG1] != 'E' || f_header.e_ident[EI_MAG2] != 'L' || f_header.e_ident[EI_MAG3] != 'F') {
		printf("\nError: Not an ELF file.\n");
		exit(0);
	}

	// assert that file is 64-bit
	if (f_header.e_ident[EI_CLASS] != 2) {
		printf("\nError: Not 64-bit.\n");
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

	printf("\n============================================================\n");
	printf("File Header:\n");
	printf("============================================================\n\n");

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

	printf("============================================================\n");
	printf("Program Headers:\n");
	printf("============================================================\n\n");

	// handle program headers
	for (int program = 0; program < f_header.e_phnum; program++) {

		// move to program header offset
		fseek(f_ptr, f_header.e_phoff + (program * f_header.e_phentsize), SEEK_SET);

		// read header data into struct
		fread(&p_header, sizeof(char), f_header.e_phentsize, f_ptr);

		// limit number of bytes to print to at most 32 bytes
		if (p_header.p_filesz >= 32) {
			display_num_bytes = 32;
		} else {
			display_num_bytes = p_header.p_filesz;
		}

		// print program header information
		printf("Program header #%d:\n", program);
		printf(" * segment type %#010x\n", p_header.p_type);
		printf(" * virtual address of segment %#018lx\n", p_header.p_vaddr);
		printf(" * size in file %lu bytes\n", p_header.p_filesz);
		printf(" * first %d bytes starting at file offset %#018lx:\n", display_num_bytes, p_header.p_offset);
		
		// print first (up to) 32 bytes of file
		fseek(f_ptr, p_header.p_offset, SEEK_SET);
		for (int i = 0; i < display_num_bytes; i++) {
			seg_byte = fgetc(f_ptr);
			printf("%02x ", seg_byte);
			if (i == 15 || i == 31 || (i == display_num_bytes -1 && display_num_bytes < 32)) {
				printf("\n");
			}
		}
		printf("\n");
	}

	printf("============================================================\n");
	printf("Section Headers:\n");
	printf("============================================================\n\n");

	printf("%d", f_header.e_shstrndx);
	printf("%#018lx", f_header.e_shoff + (f_header.e_shstrndx * f_header.e_shentsize) + s_header.sh_name)

	// handle section headers
	for (int section = 0; section < f_header.e_shnum; section++) {

		// move to section header offset
		fseek(f_ptr, f_header.e_shoff + (section * f_header.e_shentsize), SEEK_SET);

		// read header data into struct
		fread(&s_header, sizeof(char), f_header.e_shentsize, f_ptr);

		// limit number of bytes to print to at most 32 bytes
		if (s_header.sh_size >= 32) {
			display_num_bytes = 32;
		} else {
			display_num_bytes = s_header.sh_size;
		}

		// get section name
		fseek(f_ptr, f_header.e_shoff + (f_header.e_shstrndx * f_header.e_shentsize) + s_header.sh_name, SEEK_SET);
		fgets(s_name, MAX_NAME, f_ptr);

		printf("Section header #%d:\n", section);
		printf("* section name >>%s<<\n", s_name);
		printf("* type %#04x\n", s_header.sh_type);
		printf("* virtual address of section %#018lx\n", s_header.sh_addr);
		printf("* size in file %lu bytes\n", s_header.sh_size);
		printf("* first %d bytes starting at file offset %#018lx:\n", display_num_bytes, s_header.sh_offset);

		// print first (up to) 32 bytes of file
		fseek(f_ptr, s_header.sh_offset, SEEK_SET);
		for (int i = 0; i < display_num_bytes; i++) {
			seg_byte = fgetc(f_ptr);
			printf("%02x ", seg_byte);
			if (i == 15 || i == 31 || (i == display_num_bytes -1 && display_num_bytes < 32)) {
				printf("\n");
			}
		}
		printf("\n");

	}

	// close file
	fclose(f_ptr);

	// free alloced memory
	

	return 0;
}
