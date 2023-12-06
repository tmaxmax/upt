#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

uint32_t extract_bits(uint32_t num, uint8_t pos, uint8_t count) {
    return num & ((~(uint32_t)0 >> (32 - count)) << (32 - pos - count));
}

uint32_t set_bits(uint32_t x, uint8_t poz, uint8_t n, uint32_t y) {
    return (x - extract_bits(x, poz, n)) |
           (extract_bits(y, 32 - n, n) << (32 - poz - n));
}

uint32_t setbits(uint32_t x, uint8_t poz, uint8_t n, uint32_t y) {
    uint32_t m = 1 << (31 - poz);
    uint32_t copie = 0;

    for (int i = 0; i < n; i++) {
        copie |= (x & m);
        m >>= 1;
    }

    x -= copie;

    copie = 0;
    m = 1;
    for (int i = 0; i < n; i++) {
        copie |= (y & m);
        m <<= 1;
    }

    copie <<= 32 - (poz + n);
    x |= copie;

    return x;
}

void print_binary(uint32_t num, FILE *out) {
    bool found_set_bit = false;
    for (size_t i = 0; i < (8 * sizeof num); i++) {
        const uint32_t mask = 1 << (31 - i);
        if (num & mask) {
            found_set_bit = true;
        }

        if (found_set_bit) {
            putc((num & (1 << (31 - i))) ? '1' : '0', out);
        }
    }
}

int main(void) {
    uint32_t x = 0b111111001;
    uint32_t y = 0b1001;

    print_binary(set_bits(x, 23, 4, y), stdout);
    putc('\n', stdout);
    fflush(stdout);

    return 0;
}
