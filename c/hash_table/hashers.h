#ifndef HASH_TABLE_HASHERS_H
#define HASH_TABLE_HASHERS_H

#include <stddef.h>

// Hasher for null terminated byte strings.
size_t ht_hasher_string(const void *chars);

#endif