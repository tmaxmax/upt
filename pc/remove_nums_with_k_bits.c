#include <stdio.h>

#define PC_READ_IMPLEMENTATION
#include "pc/read.h"

int count_set_bits(uint64_t num) {
    int count = 0;
    for (; num; num >>= 1) {
        count += num & 1;
    }

    return count;
}

size_t remove_nums_with_k_set_bits(uint64_t *nums, const size_t count,
                                   const int k) {
    // new_count va retine noul numar de elemente din vector dupa eliminare.
    // Algoritmul de eliminare de fapt muta valorile ce NU trebuie eliminate la
    // inceputul vectorului in ordinea in care apar. Astfel nu trebuie sa tot
    // mutam toate elementele de dupa cu o pozitie inainte de fiecare data cand
    // stergem un element.
    size_t new_count = 0;
    for (size_t i = 0; i < count; i++) {
        // Mutam in fata elementele ce nu trebuie eliminate, de aceea != k.
        if (count_set_bits(nums[i]) != k) {
            nums[new_count++] = nums[i];
        }
    }
    return new_count;
}

#define MAX_COUNT 100

int main(void) {
    size_t n;
    int k;

    if (scanf("%zu %d", &n, &k) != 2 || n > MAX_COUNT || k < 0 || k > 64) {
        // Valori invalide de intrare.
        return 1;
    }

    uint64_t nums[MAX_COUNT];
    if (read_u64s_n(nums, n, stdin) != 1) {
        // Nu au fost date destule numere sau input-ul e invalid.
        return 1;
    }

    n = remove_nums_with_k_set_bits(nums, n, k);
    for (size_t i = 0; i < n; i++) {
        printf("%llu ", nums[i]);
    }
    printf("\n");

    return 0;
}
