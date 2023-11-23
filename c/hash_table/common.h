#ifndef HASH_TABLE_COMMON_H
#define HASH_TABLE_COMMON_H

#include <stdbool.h>
#include <stddef.h>

typedef size_t (*Hasher)(const void *v);
typedef int (*Comparator)(const void *a, const void *b);
typedef const void *(*KeyOwnFunction)(const void *key);
typedef void (*FreeFunction)(const void *x);

#endif
