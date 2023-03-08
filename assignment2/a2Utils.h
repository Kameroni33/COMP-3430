#include <stdio.h>

typedef struct OutputFiles
{
    FILE *a;
    FILE *b;
    FILE *c;
    FILE *d;
    FILE *e;
    FILE *f;
    FILE *g;
    FILE *h;
    FILE *i;
    FILE *j;
    FILE *k;
    FILE *l;
    FILE *m;
    FILE *n;
    FILE *o;
    FILE *p;
    FILE *q;
    FILE *r;
    FILE *s;
    FILE *t;
    FILE *u;
    FILE *v;
    FILE *w;
    FILE *x;
    FILE *y;
    FILE *z;
    FILE *other;
} OutputFiles;

void helloWorld();

void openForAppend(FILE *file, char *path);
void initializeOutputs(struct OutputFiles outputs);
