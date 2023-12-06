#include <stdbool.h>
#include <stdio.h>

#define PC_READ_IMPLEMENTATION
#include "pc/read.h"

static uint8_t with_mirrored_bits(uint8_t num) {
    uint8_t result = 0;
    for (size_t i = 0; i < 8; i++) {
        result |= ((num & (1 << i)) ? (1 << (7 - i)) : 0);
    }

    return result;
}

#define MAX_COUNT 100

int main(void) {
    uint8_t nums[MAX_COUNT];
    size_t count;
    if (read_u8s_eof(nums, &count, MAX_COUNT, stdin) != EOF) {
        return 1;
    }

    int num_palindrom_pairs = 0;
    for (size_t i = 0; i < count; i++) {
        uint8_t mirror = with_mirrored_bits(nums[i]);
        for (size_t j = i + 1; j < count; j++) {
            if (nums[j] == mirror) {
                printf("%" PRIu8 " %" PRIu8 "\n", nums[i], nums[j]);
                num_palindrom_pairs++;
            }
        }
    }

    printf("%d\n", num_palindrom_pairs);

    return 0;
}
