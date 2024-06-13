#include <stdio.h>

int main(void) {
    int t;
    scanf("%d", &t);

    for (int i = 0; i < t; i++) {
        long n, k;
        scanf("%ld %ld", &n, &k);

        long mod[10000];
        mod[0] = n / k;
        for (int j = 1; j < k; j++) {
            mod[j] = (n - j) / k + 1;
        }

        long sum = mod[0] * (mod[0] - 1) / 2;

        for (int j = 1; j < k / 2 + k % 2; j++) {
            sum += mod[j] * mod[k - j];
        }

        if (k % 2 == 0) {
            sum += (mod[k / 2] * (mod[k / 2] - 1)) / 2;
        }

        printf("%ld\n", sum);
    }

    return 0;
}
