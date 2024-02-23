#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_INPUT_NUM 1000

typedef struct {
    uint8_t data[MAX_INPUT_NUM / CHAR_BIT + 1];
} Bitset;

Bitset bitset_new(void) { return (Bitset){{0}}; }

void bitset_set(Bitset *b, uint32_t i) {
    b->data[i / CHAR_BIT] |= 1 << (i % CHAR_BIT);
}

bool bitset_test(Bitset *b, uint32_t i) {
    return (b->data[i / CHAR_BIT] & (1 << (i % CHAR_BIT))) != 0;
}

int main(void) {
    Bitset b = bitset_new();

    for (uint32_t num; scanf("%" SCNu32 " ", &num) != EOF;) {
        bitset_set(&b, num);
    }

    for (uint32_t num = 0; num <= 1000; num++) {
        if (bitset_test(&b, num)) {
            printf("%" PRIu32 " ", num);
        }
    }

    printf("\n");

    return 0;
}
