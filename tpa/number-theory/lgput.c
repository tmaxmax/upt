#include <stdio.h>

int main(void) {
    FILE *in = fopen("lgput.in", "r");
    FILE *out = fopen("lgput.out", "w");

    long n, p;
    fscanf(in, "%ld %ld", &n, &p);

    const long mod = 1999999973L;

    long r = 1, s = n;
    while (p > 0) {
        if (p % 2 == 1) {
            r = (r * s) % mod;
        }
        s = (s * s) % mod;
        p /= 2;
    }

    fprintf(out, "%ld\n", r);

    fclose(in);
    fclose(out);

    return 0;
}
