#include <stdio.h>   // fscanf, printf
#include <stdlib.h>  // exit
#include <assert.h>  // assert (obv)
#include <fcntl.h>   // open, and the O_stuff
#include <string.h>  // strlen
#include <unistd.h>  // write


int main(int argc, char *argv[]) {
    assert(argc > 1);

    // first, see all the arguments
    for (int i = 1; i < argc; i++){
        printf("Found file %s\n",argv[i]);
    }

    // just open this one :P
    FILE *file = fopen("wordgetter.c", "r");
    char word[100];

    // read-write, truncate if there is a file there, create one
    // if there is no file there.
    int outputFile = open("output.txt", O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR );

    if (file == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    while (fscanf(file, "%s", word) != EOF) {
        printf("%s\n", word);
        // one per line
        // two writes, one of the word, then the newline
        write(outputFile, word, strlen(word));
        write(outputFile, "\n", 1);
    }

    fclose(file);
    return 0;
}
