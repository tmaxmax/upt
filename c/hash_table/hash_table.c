#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../die.h"
#include "bucket.h"
#include "common.h"
#include "hash_table.h"

void noop_free(const void *x) { (void)x; }
const void *noop_key_own(const void *key) { return key; }

static const double max_load_factor = 0.8;
static const double min_load_factor = 0.2; // max_load_factor / 4

struct Impl {
    struct Bucket *buckets;
    size_t num_buckets;
    size_t num_elements;
    Hasher hash;
    Comparator key_cmp;
    KeyOwnFunction key_own;
    FreeFunction key_free;
    bool auto_resize;
};

HashTable ht_new(Hasher hash, Comparator key_cmp, KeyOwnFunction key_own,
                 FreeFunction key_free) {
    const size_t initial_num_buckets = 10;

    struct Impl *ht = malloc(sizeof(struct Impl));
    ht->buckets = calloc(initial_num_buckets, sizeof(struct Bucket));
    if (ht->buckets == NULL) {
        die("out of memory");
    }

    ht->num_buckets = initial_num_buckets;
    ht->num_elements = 0;
    ht->hash = hash;
    ht->key_cmp = key_cmp;
    ht->auto_resize = true;

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

    HashTable wrapper;
    wrapper.impl = ht;

    return wrapper;
}

static double ht_load_factor(struct Impl *ht) {
    return (double)ht->num_elements / (double)ht->num_buckets;
}

static void ht_resize(struct Impl *ht, size_t new_num_buckets) {
    if (new_num_buckets == ht->num_buckets) {
        return;
    }

    struct Bucket *new_buckets = calloc(new_num_buckets, sizeof(struct Bucket));
    if (new_buckets == NULL) {
        die("out of memory");
    }

    for (size_t i = 0; i < ht->num_buckets; i++) {
        for (struct BucketEntry *e = ht->buckets[i].start; e != NULL;) {
            const size_t new_idx = ht->hash(e->key) % new_num_buckets;
            bucket_add(&new_buckets[new_idx], e->key, e->value, ht->key_cmp,
                       noop_key_own, false);
            struct BucketEntry *curr = e;
            e = e->next;
            free_bucket_entry(curr, NULL, NULL);
        }
    }

    free(ht->buckets);
    ht->buckets = new_buckets;
    ht->num_buckets = new_num_buckets;
}

static InsertResult ht_insert_impl(struct Impl *ht, const void *key,
                                   void *value, bool can_replace) {
    if (ht->auto_resize && ht_load_factor(ht) >= max_load_factor) {
        ht_resize(ht, ht->num_buckets * 2);
    }

    const size_t idx = ht->hash(key) % ht->num_buckets;
    InsertResult res = bucket_add(&ht->buckets[idx], key, value, ht->key_cmp,
                                  ht->key_own, can_replace);
    ht->num_elements += res.is_new;

    return res;
}

InsertResult ht_insert(HashTable ht, const void *key, void *value) {
    return ht_insert_impl(ht.impl, key, value, false);
}

InsertResult ht_upsert(HashTable ht, const void *key, void *value) {
    return ht_insert_impl(ht.impl, key, value, true);
}

void *ht_remove(HashTable w, const void *key) {
    struct Impl *ht = w.impl;

    if (ht->auto_resize && ht_load_factor(ht) < min_load_factor) {
        ht_resize(ht, ht->num_buckets / 2);
    }

    const size_t idx = ht->hash(key) % ht->num_buckets;
    void *removed_value =
        bucket_remove(&ht->buckets[idx], key, ht->key_cmp, ht->key_free);
    ht->num_elements -= removed_value != NULL;

    return removed_value;
}

void *ht_get_or_insert(HashTable w, const void *key, void *default_value) {
    struct Impl *ht = w.impl;
    const size_t idx = ht->hash(key) % ht->num_buckets;
    return bucket_find_or_insert_value(&ht->buckets[idx], key, default_value,
                                       ht->key_cmp, ht->key_own);
}

void *ht_get(HashTable ht, const void *key) {
    return ht_get_or_insert(ht, key, NULL);
}

bool ht_has(HashTable ht, const void *key) { return ht_get(ht, key) != NULL; }

void ht_for_each(HashTable w, void *data,
                 bool (*fn)(void *data, const void *key, void *value)) {
    struct Impl *ht = w.impl;
    ht->auto_resize = false;

    for (size_t i = 0; i < ht->num_buckets; i++) {
        for (struct BucketEntry *e = ht->buckets[i].start; e != NULL;) {
            struct BucketEntry *curr = e;
            e = e->next;
            if (!fn(data, curr->key, curr->value)) {
                break;
            }
        }
    }

    ht_rehash(w, 0);
    ht->auto_resize = true;
}

void ht_free(HashTable w, FreeFunction value_free) {
    struct Impl *ht = w.impl;
    for (size_t i = 0; i < ht->num_buckets; i++) {
        for (struct BucketEntry *e = ht->buckets[i].start; e != NULL;) {
            struct BucketEntry *curr = e;
            e = e->next;
            free_bucket_entry(curr, ht->key_free, value_free);
        }
    }

    free(ht->buckets);
    free(ht);
}

size_t ht_size(HashTable w) { return ((struct Impl *)w.impl)->num_elements; }

void ht_rehash(HashTable w, size_t num_buckets) {
    struct Impl *ht = w.impl;

    size_t new_num_buckets =
        (size_t)((double)ht->num_elements / max_load_factor) + 1;
    if (new_num_buckets < num_buckets) {
        new_num_buckets = num_buckets;
    }

    ht_resize(ht, new_num_buckets);
}

void ht_reserve(HashTable w, size_t required_size) {
    ht_rehash(w, (size_t)((double)required_size / max_load_factor) + 1);
}