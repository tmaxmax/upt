#ifndef HASH_TABLE_BUCKET_H
#define HASH_TABLE_BUCKET_H

#include "common.h"

struct BucketEntry {
    const void *key;
    void *value;

    struct BucketEntry *next;
};

struct BucketEntry *new_bucket_entry(const void *owned_key, void *value);
void free_bucket_entry(struct BucketEntry *e, FreeFunction key_free,
                       FreeFunction value_free);

struct Bucket {
    struct BucketEntry *start;
};

typedef struct {
    void *value;
    bool is_new;
} InsertResult;

InsertResult bucket_insert(struct Bucket *b, const void *key, void *value,
                           Comparator key_cmp, KeyOwnFunction key_own,
                           bool can_replace);
void *bucket_remove(struct Bucket *b, const void *key, Comparator key_cmp,
                    FreeFunction key_free);
InsertResult bucket_find_or_insert(struct Bucket *b, const void *key,
                                   void *default_value, Comparator key_cmp,
                                   KeyOwnFunction key_own);

#endif
