#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct overlay1 {
	uint8_t one;
	uint8_t two;
	uint8_t three;
	char words[11];
};

struct overlay2 {
	uint8_t one;
	uint8_t two;
};

struct overlay3 {
	uint16_t oneandtwo;
	uint8_t three;
	char words_part1[5];
	char words_space[1];
	char words_part2[5];
};


int main() {
	struct overlay1* data = malloc(sizeof(struct overlay1));
	
	// populate with data
	data->one = 1;
	data->two = 2;
	data->three = 3;

	strcpy(data->words, "hello world");

	printf("Data: 1: %d, 2:%d, 3:%d words:%s\n", data->one, data->two, data->three, data->words);

	// we can do a partial overlay
	struct overlay2* data2 = (struct overlay2*)data;
	printf("Data: 1: %d, 2:%d - no more!\n", data2->one, data2->two);

	// or break up the slab of data differently
	struct overlay3* data3 = (struct overlay3*)data;
	printf("Data: 2<<8+1 (endianess shows itself!): %x=%x, 3:%d words:%s %s\n", (data->two<<8)+data->one,
		data3->oneandtwo, data3->three, data3->words_part1, data3->words_part2);

}