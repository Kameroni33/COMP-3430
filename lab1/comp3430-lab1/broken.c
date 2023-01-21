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
    Data d[DATA_LENGTH];
    Data* data [DATA_LENGTH] = {&d[0], &d[0], &d[0], &d[0], &d[0], &d[0], &d[0], &d[0], &d[0], &d[0]};
    for (i = 0; i < DATA_LENGTH; i++)
    {
        data[i]->number = i;
        sprintf("%x %d %s\n", data[i]->name, data[i]->number, a);
    }
    printf("%s\n", data[8]->name);

}
