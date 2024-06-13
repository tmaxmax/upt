#include <inttypes.h>
#include <stdio.h>

int main(void) {
    int a;
    uint64_t b;

    scanf("%d %" SCNu64, &a, &b);

    // Hackerrank 8/10
    // 2^a = 1 + 2^(a-1) + 2^(a-2) + ... + 2^0
    // 2^(2^a) = 2^(1 + 1 + 2 + 4 + ... + 2^(a-1)) =
    //         = 2 * (2^(2^0)) * (2^(2^1)) * (2^(2^2)) * ... * (2^(2^(a-1))) =
    //         = 2 * 2 * (2^2) * (2^4) * (2^8) * ... * (2^(2^(a-1)))
    // x(0) = 2
    // x(n) = x(n-1) * x(n-1)

    uint64_t r = 2;
    uint64_t x = 2;

    for (int i = 0; i < a; i++) {
        r = (r * x) % b;
        x = (x * x) % b;
    }

    printf("%" PRIu64 "\n", r);

    return 0;
}
