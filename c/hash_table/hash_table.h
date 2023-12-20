#ifndef HASH_TABLE_HASH_TABLE_H
#define HASH_TABLE_HASH_TABLE_H

#include <stdbool.h>
#include <stddef.h>

typedef size_t (*Hasher)(const void *v);
typedef int (*Comparator)(const void *a, const void *b);
typedef const void *(*KeyOwnFunction)(const void *key);
typedef void (*FreeFunction)(const void *x);

const void *noop_key_own(const void *key);
void noop_free(const void *x);

typedef struct {
    void *impl;
} HashTable;

// Creates a new hash table.
//
// The Hasher is used to get the key hash and the Comparator to compare keys.
// The KeyOwnFunction should return a heap-allocated copy of the key,
// of which the HashTable becomes responsible. The FreeFunction is used
// to free the owned keys.
//
// The hash table has 10 buckets preallocated by default.
// Use ht_rehash or ht_reserve to change that.
HashTable ht_new(Hasher hash, Comparator key_cmp, KeyOwnFunction key_own,
                 FreeFunction key_free);

typedef struct {
    void *value;
    bool is_new;
} InsertResult;

// Inserts the value into the hash table with the given corresponding key.
// If the given key already has a value, nothing is done and the old value is
// returned. Otherwise the new value is returned. A flag which indicates
// whether the key is new or not is also returned.
InsertResult ht_insert(HashTable ht, const void *key, void *value);

// Inserts the value into the hash table with the corresponding key
// and replaces the old value with the new value, if the key already had a
// value. Returns the inserted value and a flag which indicates whether
// the key is new or not.
InsertResult ht_upsert(HashTable ht, const void *key, void *value);

// Gets the value at the given key or inserts the provided value.
// Returns the value at the key.
void *ht_get_or_insert(HashTable ht, const void *key, void *default_value);

// Gets the value at the given key. Returns NULL if there is no value.
void *ht_get(const HashTable ht, const void *key);

// Removes the value at the given key and returns it.
// Returns NULL if no value was removed.
void *ht_remove(HashTable ht, const void *key);

// Checks if a key has a corresponding value.
bool ht_has(const HashTable ht, const void *key);

// Loops through each entry in the hash table in an indeterminate order.
// The data parameter can be used to pass additional data to the function,
// as if it would be a closure.
//
// Do not insert or remove entries while iterating – if the hash table
// is rehashed, the iteration will be broken and invalid memory will be
// accessed.
void ht_for_each(const HashTable ht, void *data,
                 bool (*fn)(void *data, const void *key, void *value));

// Deallocates the hash table. If a FreeFunction is also provided,
// the values inside the hash table will be deallocated aswell.
void ht_free(HashTable ht, FreeFunction value_free);

// Returns the number of elements in the hash table.
size_t ht_size(const HashTable ht);

// Reserves at least the specified number of buckets and regenerates the hash
// table.
void ht_rehash(HashTable ht, size_t num_buckets);

// Reserves space for at least the specified number of elements and regenerates
// the hash table. In other words, it rehashes the table with a sufficient
// number of buckets so that the maximum load factor is not exceeded for the
// required size.
void ht_reserve(HashTable ht, size_t required_size);

#endif
