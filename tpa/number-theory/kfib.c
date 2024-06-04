#include <stdio.h>

typedef struct {
    long a, b, c, d;
} Z;

typedef struct {
    long f0, f1;
} M;

Z z_mul(const Z *a, const Z *b, long mod) {
    Z r = {(a->a * b->a + a->b * b->c) % mod, (a->a * b->b + a->b * b->d) % mod,
           (a->c * b->a + a->d * b->c) % mod,
           (a->c * b->b + a->d * b->d) % mod};

    return r;
}

Z z_pow(int n, long mod) {
    Z z0 = {0, 1, 1, 1};

    Z r = {1, 0, 0, 1};
    Z s = z0;

    while (n > 0) {
        if (n % 2 == 1) {
            r = z_mul(&r, &s, mod);
        }
        s = z_mul(&s, &s, mod);
        n /= 2;
    }

    return r;
}

M z_m(const Z *z, const M *m, long mod) {
    M r = {(m->f0 * z->a + m->f1 * z->c) % mod,
           (m->f0 * z->b + m->f1 * z->d) % mod};

    return r;
}

int main(void) {
    FILE *in = fopen("kfib.in", "r");
    FILE *out = fopen("kfib.out", "w");

    int k;
    fscanf(in, "%d", &k);

    const long mod = 666013;
    Z z = z_pow(k - 1, mod);
    M m0 = {0, 1};
    M m = z_m(&z, &m0, mod);

    fprintf(out, "%ld\n", m.f1);

    fclose(in);
    fclose(out);

    return 0;
}
