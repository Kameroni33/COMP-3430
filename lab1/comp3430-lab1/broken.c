#include <stdio.h>
#include <stdlib.h>

#define DATA_LENGTH 10

typedef struct {
    int number;
    char* name;
} Data;

int main()
{
    int i;
    char a[] = "Hello";
    Data* data [DATA_LENGTH];
    for (i = 0; i < DATA_LENGTH; i++)
    {
        data[i] = malloc(sizeof(Data));
        data[i]->number = i;
        data[i]->name = malloc(sizeof(char[7]));
        sprintf(data[i]->name, "%d %s\n", data[i]->number, a);
    }
    printf("%s\n", data[8]->name);

}
