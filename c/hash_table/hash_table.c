#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../die.h"
#include "hash_table.h"

//
// ---- Bucket ----

struct Bucket {
    const void *key;
    void *value;

    struct Bucket *next;
};

static struct Bucket *new_bucket(const void *owned_key, void *value) {
    struct Bucket *b = malloc(sizeof(struct Bucket));
    if (b == NULL) {
        die("out of memory");
    }

    b->key = owned_key;
    b->value = value;
    b->next = NULL;

    return b;
}

static void free_bucket(struct Bucket *b, FreeFunction key_free,
                        FreeFunction value_free) {
    if (key_free != NULL) {
        key_free(b->key);
    }
    if (value_free != NULL) {
        value_free(b->value);
    }
    free(b);
}

static InsertResult bucket_insert(struct Bucket *b, const void *key,
                                  void *value, Comparator key_cmp,
                                  KeyOwnFunction key_own, bool can_replace) {
    if (b->key == NULL) {
        b->key = key_own(key);
        b->value = value;
        return (InsertResult){.value = value, .is_new = true};
    } else if (key_cmp(key, b->key) == 0) {
        if (can_replace) {
            b->value = value;
        }
        return (InsertResult){.value = b->value, .is_new = false};
    }

    struct Bucket *prev = NULL;
    for (; b != NULL; prev = b, b = b->next) {
        if (key_cmp(b->key, key) == 0) {
            if (can_replace) {
                b->value = value;
            }

            return (InsertResult){.value = b->value, .is_new = false};
        }
    }

    prev->next = new_bucket(key_own(key), value);

    return (InsertResult){.value = value, .is_new = true};
}

static void *bucket_remove(struct Bucket *b, const void *key,
                           Comparator key_cmp, FreeFunction key_free) {
    if (b->key != NULL && key_cmp(b->key, key) == 0) {
        void *val = b->value;
        key_free(b->key);
        if (b->next == NULL) {
            b->key = NULL;
            b->value = NULL;
        } else {
            *b = *b->next;
        }

        return val;
    }

    struct Bucket *prev = b;
    for (b = b->next; b != NULL; prev = b, b = b->next) {
        if (key_cmp(b->key, key) != 0) {
            continue;
        }

        prev->next = b->next;
        void *val = b->value;
        free_bucket(b, key_free, NULL);

        return val;
    }

    return NULL;
}

static inline InsertResult
bucket_find_or_insert(struct Bucket *b, const void *key, void *default_value,
                      Comparator key_cmp, KeyOwnFunction key_own) {
    if (b->key == NULL) {
        if (default_value == NULL) {
            return (InsertResult){.value = NULL, .is_new = false};
        }

        b->key = key_own(key);
        b->value = default_value;

        return (InsertResult){.value = default_value, .is_new = true};
    } else if (key_cmp(key, b->key) == 0) {
        return (InsertResult){.value = b->value, .is_new = false};
    }

    struct Bucket *prev = b;
    for (b = b->next; b != NULL; prev = b, b = b->next) {
        if (key_cmp(b->key, key) == 0) {
            return (InsertResult){.value = b->value, .is_new = false};
        }
    }

    if (default_value == NULL) {
        return (InsertResult){.value = NULL, .is_new = false};
    }

    prev->next = new_bucket(key_own(key), default_value);

    return (InsertResult){.value = default_value, .is_new = true};
}

//
// ---- HashTable ----

void noop_free(const void *x) { (void)x; }
const void *noop_key_own(const void *key) { return key; }

static const double max_load_factor = 0.8;
static const double min_load_factor = 0.2; // max_load_factor / 4
static const size_t initial_num_buckets = 2;

struct Impl {
    struct Bucket *buckets;
    size_t num_buckets;
    size_t num_elements;
    Hasher hash;
    Comparator key_cmp;
    KeyOwnFunction key_own;
    FreeFunction key_free;
};

HashTable ht_new(Hasher hash, Comparator key_cmp, KeyOwnFunction key_own,
                 FreeFunction key_free) {
    struct Impl *ht = malloc(sizeof(struct Impl));

    ht->buckets = NULL;
    ht->num_buckets = 0;
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

    HashTable wrapper;
    wrapper.impl = ht;

    return wrapper;
}

static double ht_load_factor(struct Impl *ht) {
    if (ht->num_buckets == 0) {
        return 1;
    }
    return (double)ht->num_elements / (double)ht->num_buckets;
}

static void ht_resize(struct Impl *ht, size_t new_num_buckets) {
    if (new_num_buckets == ht->num_buckets) {
        return;
    }

    struct Bucket *new_buckets = NULL;

    if (new_num_buckets > 0) {
        new_buckets = calloc(new_num_buckets, sizeof(struct Bucket));
        if (new_buckets == NULL) {
            die("out of memory");
        }

        for (size_t i = 0; i < ht->num_buckets; i++) {
            struct Bucket *first_bucket = &ht->buckets[i];
            if (first_bucket->key == NULL) {
                continue;
            }

            for (struct Bucket *e = first_bucket; e != NULL;) {
                struct Bucket *curr = e;
                e = e->next;
                curr->next = NULL;

                const size_t new_idx = ht->hash(curr->key) % new_num_buckets;
                struct Bucket *b = &new_buckets[new_idx];
                if (b->key == NULL) {
                    *b = *curr;
                    if (curr != first_bucket) {
                        free_bucket(curr, NULL, NULL);
                    }
                } else {
                    struct Bucket *add_after = b;
                    for (; add_after->next != NULL; add_after = add_after->next)
                        ;
                    if (curr != first_bucket) {
                        add_after->next = curr;
                    } else {
                        add_after->next = new_bucket(curr->key, curr->value);
                    }
                }
            }
        }
    }

    if (ht->buckets != NULL) {
        free(ht->buckets);
    }

    ht->buckets = new_buckets;
    ht->num_buckets = new_num_buckets;
}

static InsertResult ht_insert_impl(struct Impl *ht, const void *key,
                                   void *value, bool can_replace) {
    if (value == NULL) {
        return (InsertResult){.value = NULL, .is_new = false};
    } else if (ht->num_buckets == 0) {
        ht_resize(ht, initial_num_buckets);
    }

    const size_t idx = ht->hash(key) % ht->num_buckets;
    InsertResult res = bucket_insert(&ht->buckets[idx], key, value, ht->key_cmp,
                                     ht->key_own, can_replace);
    ht->num_elements += res.is_new;

    if (ht_load_factor(ht) >= max_load_factor) {
        ht_resize(ht, ht->num_buckets * 2 + 1);
    }

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

    if (ht->num_buckets == 0) {
        return NULL;
    }

    const size_t idx = ht->hash(key) % ht->num_buckets;
    void *removed_value =
        bucket_remove(&ht->buckets[idx], key, ht->key_cmp, ht->key_free);
    ht->num_elements -= removed_value != NULL;

    if (ht_load_factor(ht) < min_load_factor) {
        ht_resize(ht, ht->num_buckets / 2);
    }

    return removed_value;
}

void *ht_get_or_insert(HashTable w, const void *key, void *default_value) {
    struct Impl *ht = w.impl;

    if (ht->num_buckets == 0) {
        if (default_value != NULL) {
            return ht_insert(w, key, default_value).value;
        }
        return NULL;
    }

    const size_t idx = ht->hash(key) % ht->num_buckets;
    InsertResult res = bucket_find_or_insert(
        &ht->buckets[idx], key, default_value, ht->key_cmp, ht->key_own);

    if (res.is_new) {
        ht->num_elements++;

        if (ht_load_factor(ht) >= max_load_factor) {
            ht_resize(ht, ht->num_buckets * 2 + 1);
        }
    }

    return res.value;
}

void *ht_get(HashTable w, const void *key) {
    return ht_get_or_insert(w, key, NULL);
}

bool ht_has(HashTable ht, const void *key) { return ht_get(ht, key) != NULL; }

void ht_for_each(HashTable w, void *data,
                 bool (*fn)(void *data, const void *key, void *value)) {
    struct Impl *ht = w.impl;

    if (ht->num_elements == 0) {
        return;
    }

    for (size_t i = 0; i < ht->num_buckets; i++) {
        struct Bucket *e = &ht->buckets[i];
        if (e->key == NULL) {
            continue;
        }

        while (e != NULL) {
            struct Bucket *curr = e;
            e = e->next;
            if (!fn(data, curr->key, curr->value)) {
                break;
            }
        }
    }
}

void ht_free(HashTable w, FreeFunction value_free) {
    struct Impl *ht = w.impl;
    for (size_t i = 0; i < ht->num_buckets; i++) {
        struct Bucket *b = &ht->buckets[i];
        if (b->key == NULL) {
            continue;
        }

        ht->key_free(b->key);
        if (value_free != NULL) {
            value_free(b->value);
        }

        for (b = b->next; b != NULL;) {
            struct Bucket *curr = b;
            b = b->next;
            free_bucket(curr, ht->key_free, value_free);
        }
    }

    free(ht->buckets);
    free(ht);
    w.impl = NULL;
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
