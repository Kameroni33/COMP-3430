#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char** argv) {

	// read file name from command line or set default to be this file (ie. "a1q1")
	char* f_name;
	if (argc > 1) {
		f_name = argv[1];
	} else {
		f_name = argv[0];
	}

	// open file
	FILE* f_ptr;
	f_ptr = fopen(f_name, "r");
	if (f_ptr == NULL) {
		printf("%s could not be opened.\n", f_name);
		return 0;
	}

	// byte 0x04 determines 32- or 64-bit
	fseek(f_ptr, 0x04, SEEK_SET);
	char f_bit = fgetc(f_ptr);
	if (f_bit != 2) {
		printf("\nELF file must be 64-bit.\n");
		return 0;
	}

	// byte 0x05 determines endianness
	char f_endian = fgetc(f_ptr);
	char f_endianstr[8];
        if (f_endian == 1) {
                strcpy(f_endianstr, "little");
        } else if (f_endian == 2) {
                strcpy(f_endianstr, "big");
        } else {
                strcpy(f_endianstr, "unknown");
        }

	// byte 0x07 determines operating system ABI
	fseek(f_ptr, 1, SEEK_CUR);
	char f_abi = fgetc(f_ptr);

	// bytes 0x10 and 0x11 determine object file type
	fseek(f_ptr, 8, SEEK_CUR);
	short* f_objtype = malloc(sizeof(short));
	fread(f_objtype, 2, 1, f_ptr);
	
	// bytes 0x12 and 0x13 determine instruction set architecture (isa)
	short* f_isa = malloc(sizeof(short));
	fread(f_isa, 2, 1,f_ptr);

	// bytes 0x18 to 0x1F hold the memory address of the entry point
	fseek(f_ptr, 4, SEEK_CUR);
	long* f_entrypoint = malloc(sizeof(long));
	fread(f_entrypoint, 8, 1, f_ptr);

	// bytes 0x20 to 0x27 are the memory address of the program header table
	long* f_programheader = malloc(sizeof(long));
	fread(f_programheader, 8, 1, f_ptr);

	// bytes 0x28 to 0x2F are memory address of the section header table
	long* f_sectionheader = malloc(sizeof(long));
	fread(f_sectionheader, 8, 1, f_ptr);

	// bytes 0x36 and 0x37 contain the size of a program header table entry
	fseek(f_ptr, 6, SEEK_CUR);
	short* f_programheader_size = malloc(sizeof(short));
	fread(f_programheader_size, 2, 1, f_ptr);

	// bytes 0x38 and 0x39 contain the number of entries in the program header table
	short* f_programheader_num = malloc(sizeof(short));
	fread(f_programheader_num, 2, 1, f_ptr);

	// bytes 0x3A and 0x3B contain the size of a section header table entry
	short* f_sectionheader_size = malloc(sizeof(short));
	fread(f_sectionheader_size, 2, 1, f_ptr);

	// bytes 0x3C and 0x3D conatin the number of entries in the section header table
	short* f_sectionheader_num = malloc(sizeof(short));
	fread(f_sectionheader_num, 2, 1, f_ptr);

	// bytes 0x3E and 0X3F contain the index of the string table
	short* f_stringtable = malloc(sizeof(short));
	fread(f_stringtable, 2, 1, f_ptr);

	printf("ELF Header:\n"
		" * 64-bit\n"  // since we have not exited execution, we can assume the file is 64-bit
		" * %s endian\n"
		" * compiled for 0x%02d (operating system)\n"
		" * object type is 0x%02d\n"
		" * compiled for 0x%02d (isa)\n"
		" * entry point address is 0x%016lu\n"
		" * program header table starts at 0x%016lu\n"
		" * section header table starts at 0x%016lu\n"
		" * there are %d program headers, each is %d bytes\n"
		" * there are %d section headers, each is %d bytes\n"
		" * the section header string table is entry %d\n\n",
		f_endianstr, f_abi, *f_objtype, *f_isa, *f_entrypoint, *f_programheader, *f_sectionheader,
		*f_programheader_num, *f_programheader_size, *f_sectionheader_num, *f_sectionheader_size,
		*f_stringtable);

	// print out information about each program header
	for (int i = 0; i < *f_programheader_num; i++) {
		
		// bytes 0x00 to 0x03 identifies the type of segment
		fseek(f_ptr, (*f_programheader + (*f_programheader_size * i)), SEEK_SET);
		unsigned int* segmentType = malloc(sizeof(int));
		fread(segmentType, 4, 1, f_ptr);

		// bytes 0x08 to 0x0F is the offest of the segment in memory
		fseek(f_ptr, 4, SEEK_CUR);
		unsigned long* segOffset = malloc(sizeof(long));
		fread(segOffset, 8, 1, f_ptr);

		// bytes 0x10 to 0x17 hold the virtual address of the segment in memory
		unsigned long* virtualAddress = malloc(sizeof(long));
		fread(virtualAddress, 8, 1, f_ptr);

		// bytes 0x20 to 0x28 is the size of the segement in the file image
		fseek(f_ptr, 8, SEEK_CUR);
		unsigned long* sizeOnFileImage = malloc(sizeof(long));
		fread(sizeOnFileImage, 8, 1, f_ptr);

		int displayNumBytes = 32;
		if (*sizeOnFileImage < 32) {
			displayNumBytes = *sizeOnFileImage;
		}

		printf("Program header #%d:\n"
			" * segment type 0x%08x\n"
			" * virtual address of segment 0x%016lx\n"
			" * size in file %lu bytes\n"
			" * first %d bytes starting at file offset 0x%016lx:\n",
			i, *segmentType, *virtualAddress, *sizeOnFileImage, displayNumBytes, *segOffset);
		
		// print first (up to) 32 bytes of file
		fseek(f_ptr, *segOffset, SEEK_SET);
		unsigned char segByte;
		for (int i = 0; i <displayNumBytes; i++) {
			segByte = fgetc(f_ptr);
			printf("%02x ", segByte);
			if (i == 15) {
				printf("\n");
			}
		}

		printf("\n\n");

		// free allocated memory
		free(segmentType);
		free(segOffset);
		free(virtualAddress);
		free(sizeOnFileImage);
	}


	// close file
	fclose(f_ptr);

	// free alloced memory
	

	return 0;
}
