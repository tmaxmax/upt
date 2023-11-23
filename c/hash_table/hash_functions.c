#include "hash_functions.h"

size_t ht_fnv1a(const unsigned char *input, const size_t seed) {
    size_t hash = seed ^ 2166136261UL;
    unsigned char c;

    while ((c = *input++)) {
        hash ^= c;
        hash *= 16777619;
    }

    return hash;
}
