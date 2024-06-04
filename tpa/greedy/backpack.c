#include <stdio.h>
#include <stdlib.h>

typedef struct {
    double ratio;
    int index;
} Object;

static int object_sorter_desc(const void *va, const void *vb) {
    const Object *a = va;
    const Object *b = vb;

    if (a->ratio < b->ratio) {
        return 1;
    }
    if (a->ratio > b->ratio) {
        return -1;
    }
    return 0;
}

int solve_backpack(const int *weights, const int *profits, const int n,
                   const int max_weight) {
    Object *objs = malloc((unsigned)n * sizeof *objs);
    for (int i = 0; i < n; i++) {
        objs[i].index = i;
        objs[i].ratio = (double)profits[i] / (double)weights[i];
    }

    qsort(objs, (unsigned)n, sizeof *objs, object_sorter_desc);

    int profit = 0, weight = 0;
    for (int i = 0; i < n; i++) {
        int new_weight = weight + weights[objs[i].index];
        if (new_weight > max_weight) {
            continue;
        }

        weight = new_weight;
        profit += profits[objs[i].index];
    }

    return profit;
}
