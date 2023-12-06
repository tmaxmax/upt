#ifndef HASH_TABLE_HASHERS_H
#define HASH_TABLE_HASHERS_H

#include <stddef.h>

// Hasher for null terminated byte strings.
size_t ht_hasher_string(const void *chars);

// Hasher for uint8_t. The pointer is the uint8_t.
size_t ht_hasher_u8(const void *num);

// Hasher for bool. The pointer is the bool.
size_t ht_hasher_boolean(const void *b);

#endif
