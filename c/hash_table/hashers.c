#include <time.h>

#include "hash_functions.h"
#include "hashers.h"

size_t ht_hasher_chars(const void *chars) {
    static size_t initial_seed = 0;
    if (initial_seed == 0) {
        initial_seed = time(NULL);
    }

    return ht_fnv1a(chars, initial_seed);
}