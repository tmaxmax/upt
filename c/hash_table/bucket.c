#include <stdbool.h>
#include <stddef.h>

#include "../die.h"
#include "bucket.h"

struct Bucket *new_bucket(const void *owned_key, void *value) {
    struct Bucket *b = malloc(sizeof(struct Bucket));
    if (b == NULL) {
        die("out of memory");
    }

    b->key = owned_key;
    b->value = value;
    b->next = NULL;

    return b;
}

void free_bucket(struct Bucket *b, FreeFunction key_free,
                 FreeFunction value_free) {
    if (key_free != NULL) {
        key_free(b->key);
    }
    if (value_free != NULL) {
        value_free(b->value);
    }
    free(b);
}

static InsertResult bucket_insert_collision(struct Bucket *start,
                                            const void *key, void *value,
                                            Comparator key_cmp,
                                            KeyOwnFunction key_own,
                                            bool can_replace) {
    struct Bucket *prev = NULL;
    for (struct Bucket *b = start; b != NULL; prev = b, b = b->next) {
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

InsertResult bucket_insert(struct Bucket *b, const void *key, void *value,
                           Comparator key_cmp, KeyOwnFunction key_own,
                           bool can_replace) {
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

    return bucket_insert_collision(b, key, value, key_cmp, key_own,
                                   can_replace);
}

static void *bucket_remove_walk(struct Bucket *start, const void *key,
                                Comparator key_cmp, FreeFunction key_free) {
    struct Bucket *prev = start;
    for (struct Bucket *b = start->next; b != NULL; prev = b, b = b->next) {
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

void *bucket_remove(struct Bucket *b, const void *key, Comparator key_cmp,
                    FreeFunction key_free) {
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

    return bucket_remove_walk(b, key, key_cmp, key_free);
}

static InsertResult bucket_find_or_insert_walk(struct Bucket *start,
                                               const void *key,
                                               void *default_value,
                                               Comparator key_cmp,
                                               KeyOwnFunction key_own) {
    printf("collision: %s\n", key);
    struct Bucket *prev = start;
    for (struct Bucket *b = prev->next; b != NULL; prev = b, b = b->next) {
        if (key_cmp(b->key, key) == 0) {
            return (InsertResult){.value = b->value, .is_new = false};
        }
    }

    if (default_value == NULL) {
        return (InsertResult){};
    }

    prev->next = new_bucket(key_own(key), default_value);

    return (InsertResult){.value = default_value, .is_new = true};
}

InsertResult bucket_find_or_insert(struct Bucket *b, const void *key,
                                   void *default_value, Comparator key_cmp,
                                   KeyOwnFunction key_own) {
    if (b->key == NULL) {
        b->key = key_own(key);
        b->value = default_value;
        return (InsertResult){.value = default_value, .is_new = true};
    } else if (key_cmp(key, b->key) == 0) {
        return (InsertResult){.value = b->value, .is_new = false};
    }

    return bucket_find_or_insert_walk(b, key, default_value, key_cmp, key_own);
}
