#include <stdbool.h>
#include <stddef.h>

#include "../die.h"
#include "bucket.h"

struct BucketEntry *new_bucket_entry(const void *owned_key, void *value) {
    struct BucketEntry *e = malloc(sizeof(struct BucketEntry));
    if (e == NULL) {
        die("out of memory");
    }

    e->key = owned_key;
    e->value = value;
    e->next = NULL;

    return e;
}

void free_bucket_entry(struct BucketEntry *e, FreeFunction key_free,
                       FreeFunction value_free) {
    if (key_free != NULL) {
        key_free(e->key);
    }
    if (value_free != NULL) {
        value_free(e->value);
    }
    free(e);
}

struct AddResult bucket_add(struct Bucket *b, const void *key, void *value,
                            Comparator key_cmp, KeyOwnFunction key_own,
                            bool can_replace) {
    if (b->start == NULL) {
        b->start = new_bucket_entry(key_own(key), value);
        return (struct AddResult){.value = value, .is_new = true};
    }

    struct BucketEntry *prev = NULL;
    for (struct BucketEntry *e = b->start; e != NULL; prev = e, e = e->next) {
        if (key_cmp(e->key, key) == 0) {
            if (can_replace) {
                e->value = value;
            }

            return (struct AddResult){.value = value, .is_new = false};
        }
    }

    prev->next = new_bucket_entry(key_own(key), value);

    return (struct AddResult){.value = value, .is_new = true};
}

void *bucket_remove(struct Bucket *b, const void *key, Comparator key_cmp,
                    FreeFunction key_free) {
    struct BucketEntry *prev = NULL;
    for (struct BucketEntry *e = b->start; e != NULL; prev = e, e = e->next) {
        if (key_cmp(e->key, key) != 0) {
            continue;
        }

        if (prev == NULL) {
            b->start = b->start->next;
        } else {
            prev->next = e->next;
        }

        void *val = e->value;
        free_bucket_entry(e, key_free, NULL);

        return val;
    }

    return NULL;
}

void *bucket_find_or_insert_value(struct Bucket *b, const void *key,
                                  void *default_value, Comparator key_cmp,
                                  KeyOwnFunction key_own) {
    struct BucketEntry *prev = NULL;
    for (struct BucketEntry *e = b->start; e != NULL; prev = e, e = e->next) {
        if (key_cmp(e->key, key) == 0) {
            return e->value;
        }
    }

    if (default_value == NULL) {
        return NULL;
    }

    if (prev == NULL) {
        b->start = new_bucket_entry(key_own(key), default_value);
    } else {
        prev->next = new_bucket_entry(key_own(key), default_value);
    }

    return default_value;
}
