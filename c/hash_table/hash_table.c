#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../die.h"
#include "bucket.h"
#include "common.h"
#include "hash_table.h"

void noop_free(const void *x) { (void)x; }
const void *noop_key_own(const void *key) { return key; }

HashTable *ht_new(Hasher hash, Comparator key_cmp, KeyOwnFunction key_own,
                  FreeFunction key_free) {
    const size_t initial_num_buckets = 10;

    HashTable *ht = malloc(sizeof(HashTable));
    ht->buckets = calloc(initial_num_buckets, sizeof(struct Bucket));
    if (ht->buckets == NULL) {
        die("out of memory");
    }

    ht->num_buckets = initial_num_buckets;
    ht->num_elements = 0;
    ht->hash = hash;
    ht->key_cmp = key_cmp;

    if (key_own != NULL) {
        ht->key_own = key_own;
    } else {
        ht->key_own = noop_key_own;
    }

    if (key_free != NULL) {
        ht->key_free = key_free;
    } else if (key_own == NULL) {
        die("If key_own is given, key_free must exist aswell");
    } else {
        ht->key_free = noop_free;
    }

    return ht;
}

static double ht_load_factor(HashTable *ht) {
    return (double)ht->num_elements / (double)ht->num_buckets;
}

static void ht_resize(HashTable *ht, bool is_down_resize) {
    const size_t new_num_buckets =
        is_down_resize ? (ht->num_buckets / 2) : (ht->num_buckets * 2);
    struct Bucket *new_buckets = calloc(new_num_buckets, sizeof(struct Bucket));
    if (new_buckets == NULL) {
        die("out of memory");
    }

    for (size_t i = 0; i < ht->num_buckets; i++) {
        for (struct BucketEntry *e = ht->buckets[i].start; e != NULL;) {
            const size_t new_idx = ht->hash(e->key) % new_num_buckets;
            bucket_add(&new_buckets[new_idx], e->key, e->value, ht->key_cmp,
                       noop_key_own, false);
            e = e->next;
            free_bucket_entry(e, NULL);
        }
    }

    free(ht->buckets);
    ht->buckets = new_buckets;
    ht->num_buckets = new_num_buckets;
}

static void *ht_insert_impl(HashTable *ht, const void *key, void *value,
                            bool can_replace) {
    if (ht_load_factor(ht) > 0.8) {
        ht_resize(ht, false);
    }

    const size_t idx = ht->hash(key) % ht->num_buckets;
    void *inserted_value = bucket_add(&ht->buckets[idx], key, value,
                                      ht->key_cmp, ht->key_own, can_replace);
    ht->num_elements += inserted_value != NULL && inserted_value != value;

    return inserted_value;
}

void *ht_insert(HashTable *ht, const void *key, void *value) {
    return ht_insert_impl(ht, key, value, false);
}

void *ht_upsert(HashTable *ht, const void *key, void *value) {
    return ht_insert_impl(ht, key, value, true);
}

void *ht_remove(HashTable *ht, const void *key) {
    if (ht_load_factor(ht) < 0.2) {
        ht_resize(ht, true);
    }

    const size_t idx = ht->hash(key) % ht->num_buckets;
    void *removed_value =
        bucket_remove(&ht->buckets[idx], key, ht->key_cmp, ht->key_free);
    ht->num_elements -= removed_value != NULL;

    return removed_value;
}

void *ht_get_or_insert(HashTable *ht, const void *key, void *default_value) {
    const size_t idx = ht->hash(key) % ht->num_buckets;
    return bucket_find_or_insert_value(&ht->buckets[idx], key, default_value,
                                       ht->key_cmp, ht->key_own);
}

void *ht_get(HashTable *ht, const void *key) {
    return ht_get_or_insert(ht, key, NULL);
}

bool ht_has(HashTable *ht, const void *key) { return ht_get(ht, key) != NULL; }

void ht_for_each(HashTable *ht, void *data,
                 bool (*fn)(void *data, const void *key, void *value)) {
    for (size_t i = 0; i < ht->num_buckets; i++) {
        for (struct BucketEntry *e = ht->buckets[i].start; e != NULL;
             e = e->next) {
            if (!fn(data, e->key, e->value)) {
                break;
            }
        }
    }
}

void ht_free(HashTable *ht) {
    for (size_t i = 0; i < ht->num_buckets; i++) {
        for (struct BucketEntry *e = ht->buckets[i].start; e != NULL;) {
            struct BucketEntry *curr = e;
            e = e->next;
            free_bucket_entry(curr, ht->key_free);
        }
    }

    free(ht->buckets);
    free(ht);
}
