#ifndef HASH_TABLE_HASH_TABLE_H
#define HASH_TABLE_HASH_TABLE_H

#include "bucket.h"
#include "common.h"

typedef struct {
    void *impl;
} HashTable;

HashTable ht_new(Hasher hash, Comparator key_cmp, KeyOwnFunction key_own,
                 FreeFunction key_free);
void *ht_insert(HashTable ht, const void *key, void *value);
void *ht_upsert(HashTable ht, const void *key, void *value);
void *ht_get_or_insert(HashTable ht, const void *key, void *default_value);
void *ht_get(HashTable ht, const void *key);
void *ht_remove(HashTable ht, const void *key);
bool ht_has(HashTable ht, const void *key);
void ht_for_each(HashTable ht, void *data,
                 bool (*fn)(void *data, const void *key, void *value));
void ht_free(HashTable ht);
size_t ht_size(HashTable ht);

#endif
