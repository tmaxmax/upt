#include "values.h"

union float_value {
    float f;
    void *v;
};

float ht_v_float(void *from_ht) {
    union float_value fv;
    fv.v = from_ht;
    return fv.f;
}

void *ht_v_from_float(float f) {
    union float_value fv;
    fv.f = f;
    return fv.v;
}