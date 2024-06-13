#include <inttypes.h>
#include <stdio.h>

uint64_t phi(uint64_t n) {
    uint64_t r = n;
    for (uint64_t i = 2; i * i <= n; i++) {
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

uint64_t powm(uint64_t a, uint64_t n, uint64_t mod) {
    uint64_t r = 1, s = a;

    while (n > 0) {
        if (n % 2 == 1) {
            r = (r * s) % mod;
        }
        s = (s * s) % mod;
        n /= 2;
    }

    return r;
}

uint64_t modinv(uint64_t a, uint64_t p) { return powm(a, phi(p) - 1, p); }

uint64_t modbinom(uint64_t n, uint64_t m, uint64_t p) {
    for (uint64_t tn = n, tm = m; tm > 0; tn /= p, tm /= p) {
        if (tn % p < tm % p) {
            return 0;
        }
    }

    uint64_t r = 1;
    for (; n >= p || m >= p; n /= p, m /= p) {
        r *= modbinom(n % p, m % p, p);
        r %= p;
    }

    if (m > n - m) {
        m = n - m;
    }

    uint64_t d = 1;
    for (uint64_t i = 0; i < m; i++) {
        r = r * (n - i) % p;
        d = d * (i + 1) % p;
    }

    return r * modinv(d, p) % p;
}

int main(void) {
    // Infoarena 70/100

    freopen("jap2.in", "r", stdin);
    freopen("jap2.out", "w", stdout);

    int q;
    uint64_t p;

    scanf("%" SCNu64 " %d", &p, &q);

    for (int i = 0; i < q; i++) {
        uint64_t a, b;
        scanf("%" SCNu64 " %" SCNu64, &a, &b);
        printf("%" SCNu64 "\n", modbinom(a, b, p));
    }

    return 0;
}
