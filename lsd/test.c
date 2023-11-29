#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct List {
    int *elems;
    size_t len;
};

struct Filter {
    struct List list;
    bool (*cond)(int);
    size_t idx_after_last_next_elem;
};

struct Filter filter(bool (*cond)(int), struct List list) {
    return (struct Filter){
        .cond = cond, .list = list, .idx_after_last_next_elem = 0};
}

int *filter_next(struct Filter *o) {
    for (size_t i = o->idx_after_last_next_elem; i < o->list.len; i++) {
        if (o->cond(o->list.elems[i])) {
            o->idx_after_last_next_elem = i + 1;
            return &o->list.elems[i];
        }
    }
    return NULL;
}

bool is_even(int num) { return num % 2 == 0; }

extern void f(int *, size_t);

struct Tablou100000 {
    int list[100000];
};

struct Tablou100000 create_tablou_100000(void) {
    struct Tablou100000 x;
    x.list[0] = 42;
    return x;
}

int *malloc_list(void) {
    int *list = malloc(10000 * sizeof *list);
    list[0] = 43;
    return list;
}

struct List create_list(void) {
    int *elems = malloc(5 * sizeof *elems);
    for (int i = 1; i <= 5; i++) {
        elems[i - 1] = i;
    }
    return (struct List){.elems = elems, .len = 5};
}

int main(void) {
    // struct List list = create_list();
    // struct Filter even_list = filter(is_even, list);

    // for (int *elem; (elem = filter_next(&even_list)) != NULL;) {
    //     printf("%d ", *elem);
    // }
    // struct Tablou100000 y = create_tablou_100000();
    int *list = malloc_list();
    printf("%d\n", list[0]);
    free(list);

    return 0;
}
