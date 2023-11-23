#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <stddef.h>

#define array_t(typ, size) \
    struct                 \
    {                      \
        typ ptr[size];     \
    }

#define array_at(arr, i) ((arr).ptr + (i))

#define array_get(arr, i) (*(array_at(arr, i)))

#define array_size(arr) (sizeof (arr).ptr)

#define array_len(arr) ((size_t)(array_size(arr) / sizeof (arr).ptr[0]))

#define array_decay(arr) (arr).ptr, array_len(arr)

#endif