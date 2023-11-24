#include <stddef.h>
#include <stdint.h>

#include "hashers.h"

size_t ht_hasher_string(const void *chars) {
    // Implements
    // https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function#FNV-1a_hash

    const uint8_t *data = chars;
    size_t hash = 2166136261;

    for (uint8_t c; (c = *data++);) {
        hash ^= c;
        hash *= 16777619;
    }

    return hash;
}