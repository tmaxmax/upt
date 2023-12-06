#include "values.h"

#define HT_GEN_VALUE(type, name)                                               \
    union name##_value {                                                       \
        type t;                                                                \
        void *v;                                                               \
    };                                                                         \
    type ht_v_##name(const void *from_ht) {                                    \
        union name##_value nv;                                                 \
        nv.v = from_ht;                                                        \
        return nv.t;                                                           \
    }                                                                          \
    void *ht_v_from_##name(type t) {                                           \
        union name##_value nv;                                                 \
        nv.t = t;                                                              \
        return nv.v;                                                           \
    }

HT_GEN_VALUE(float, float)
HT_GEN_VALUE(bool, boolean)
HT_GEN_VALUE(uint8_t, u8)

int ht_cmp_bool(const void *a, const void *b) {
    return (int)ht_v_boolean(a) - (int)ht_v_boolean(b);
}

int ht_cmp_u8(const void *a, const void *b) {
    return (int)ht_v_u8(a) - (int)ht_v_u8(b);
}
