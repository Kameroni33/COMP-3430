#include <stdio.h>

// Global Constants ===================================================================

#define NUM_OUTPUTS 27
#define MAX_WORD 100

// Variables & Structs ================================================================

typedef struct
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

char *outputNames[] = {
    "a.txt"
    "b.txt"
    "c.txt"
    "d.txt"
    "e.txt"
    "f.txt"
    "g.txt"
    "h.txt"
    "i.txt"
    "j.txt"
    "k.txt"
    "l.txt"
    "m.txt"
    "n.txt"
    "o.txt"
    "p.txt"
    "q.txt"
    "r.txt"
    "s.txt"
    "t.txt"
    "u.txt"
    "v.txt"
    "w.txt"
    "x.txt"
    "y.txt"
    "z.txt"
    "other.txt"
};

void helloWorld()
{
    printf("hello world\n");
}

void openForAppend(FILE *file, char *path)
{
    if ((file = fopen(path, "a")) == NULL) {
        printf("Error: unable to open file for append '%s'\n", path);
        exit(1);
    }
}

void initializeOutputs(OutputFiles outputs)
{
    openForAppend(outputs.a, "a.txt");
    openForAppend(outputs.b, "b.txt");
    openForAppend(outputs.c, "c.txt");
    openForAppend(outputs.d, "d.txt");
    openForAppend(outputs.e, "e.txt");
    openForAppend(outputs.f, "f.txt");
    openForAppend(outputs.g, "g.txt");
    openForAppend(outputs.h, "h.txt");
    openForAppend(outputs.i, "i.txt");
    openForAppend(outputs.j, "j.txt");
    openForAppend(outputs.k, "k.txt");
    openForAppend(outputs.l, "l.txt");
    openForAppend(outputs.m, "m.txt");
    openForAppend(outputs.n, "n.txt");
    openForAppend(outputs.o, "o.txt");
    openForAppend(outputs.p, "p.txt");
    openForAppend(outputs.q, "q.txt");
    openForAppend(outputs.r, "r.txt");
    openForAppend(outputs.s, "s.txt");
    openForAppend(outputs.t, "t.txt");
    openForAppend(outputs.u, "u.txt");
    openForAppend(outputs.v, "v.txt");
    openForAppend(outputs.w, "w.txt");
    openForAppend(outputs.x, "x.txt");
    openForAppend(outputs.y, "y.txt");
    openForAppend(outputs.z, "z.txt");
    openForAppend(outputs.other, "other.txt");
}

