#include <stdio.h>
#include <stdlib.h>

#define PC_READ_IMPLEMENTATION
#include "pc/read.h"

typedef struct {
    int a, b;
} Pair;

int pair_read(void *dst, FILE *src) {
    Pair *p = dst;
    return read_scanf_ret(fscanf(src, "%d %d ", &p->a, &p->b), 2);
}

int pair_cmp(const void *va, const void *vb) {
    const Pair *a = va, *b = vb;
    if (a->a != b->a) {
        return a->a - b->a;
    }
    return a->b - b->b;
}

#define MAX_COUNT 100

int main(void) {
    Pair pairs[MAX_COUNT];
    size_t count;
    if (read_to_eof_of(pairs, sizeof *pairs, &count, MAX_COUNT, stdin,
                       pair_read) != EOF) {
        return 1;
    }

    qsort(pairs, count, sizeof *pairs, pair_cmp);

    for (size_t i = 0; i < count; i++) {
        printf("%d %d\n", pairs[i].a, pairs[i].b);
    }

    return 0;
}
