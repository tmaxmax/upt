#ifndef HASH_TABLE_HASH_FUNCTIONS_H
#define HASH_TABLE_HASH_FUNCTIONS_H

#include "stddef.h"

// https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function#FNV-1a_hash
size_t ht_fnv1a(const unsigned char *input, const size_t seed);

#endif
