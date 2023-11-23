#ifndef TEO_DIE_H
#define TEO_DIE_H

#include <stdio.h>
#include <stdlib.h>

#define die(...)                                                               \
    do {                                                                       \
        fprintf(stderr, __VA_ARGS__);                                          \
        exit(2);                                                               \
    } while (0)

#endif
