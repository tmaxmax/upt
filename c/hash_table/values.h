#ifndef HASH_TABLE_VALUES_H
#define HASH_TABLE_VALUES_H

#include <stdbool.h>
#include <stdint.h>

#define HT_INTERNAL_GEN_VALUE_HEADER(type, name)                               \
    type ht_v_##name(const void *from_ht);                                     \
    void *ht_v_from_##name(type t);

#define HT_INTERNAL_GEN_KEY_HEADER(type, name)                                 \
    HT_INTERNAL_GEN_VALUE_HEADER(type, name)                                   \
    int ht_cmp_##name(const void *a, const void *b);

HT_INTERNAL_GEN_VALUE_HEADER(float, float)
HT_INTERNAL_GEN_KEY_HEADER(bool, boolean)
HT_INTERNAL_GEN_KEY_HEADER(uint8_t, u8)
HT_INTERNAL_GEN_KEY_HEADER(int, int)

#endif
