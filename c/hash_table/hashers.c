#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "hashers.h"
#include "values.h"
#include "xxhash/xxhash.h"

static int read_random_number(uint64_t *dst) {
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) {
        return fd;
    }

    const size_t expected_read = sizeof *dst;
    if (read(fd, dst, expected_read) != expected_read) {
        return -1;
    }

    return close(fd);
}

static uint64_t fnv1a_64(const uint8_t *input, size_t size, uint64_t seed) {
    uint64_t hash = seed ^ 14695981039346656037UL;

    for (const uint8_t *c = input; c < input + size; c++) {
        hash ^= *c;
        hash *= 1099511628211;
    }

    return hash;
}

size_t ht_hasher_string(const void *chars) {
    static uint64_t seed = 0;
    if (seed == 0) {
        read_random_number(&seed);
    }

    const size_t len = strlen(chars);

    if (len <= 32) {
        return fnv1a_64(chars, len, seed);
    }

    return XXH64(chars, len, seed);
}

size_t ht_hasher_u8(const void *num) { return ht_v_u8(num); }

size_t ht_hasher_boolean(const void *b) { return ht_v_boolean(b); }
