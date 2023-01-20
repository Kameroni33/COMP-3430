#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int number;
    char* name;
} Data;

int main()
{
    int i;
    char a[] = "Hello";
#define DATA_LENGTH 10
    Data* data [DATA_LENGTH];
    for (i = 0; i < DATA_LENGTH; i++)
    {
        data[i]->number = i;
        sprintf("%d %s\n", data[i]->name, data[i]->number, a);
    }
    printf("%s\n", data[8]->name);

}
