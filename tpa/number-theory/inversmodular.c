#include <stdio.h>

long phi(long n) {
    long r = n;
    for (long i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            while (n % i == 0)
                n /= i;
            r -= r / i;
        }
    }
    if (n > 1)
        r -= r / n;
    return r;
}

long powm(long a, long n, long mod) {
    long r = 1, s = a;

    while (n > 0) {
        if (n % 2 == 1) {
            r = (r * s) % mod;
        }
        s = (s * s) % mod;
        n /= 2;
    }

    return r;
}

int main(void) {
    FILE *in = fopen("inversmodular.in", "r");
    FILE *out = fopen("inversmodular.out", "w");

    long a, n;
    fscanf(in, "%ld %ld", &a, &n);

    fprintf(out, "%ld\n", powm(a, phi(n) - 1, n));

    fclose(in);
    fclose(out);

    return 0;
}
