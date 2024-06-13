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

long modinv(long a, long n) { return powm(a, phi(n) - 1, n); }

long modbinom(long a, long b, long p) {
    for (long n = a, m = b; m > 0; n /= p, m /= p) {
        if (n % p < m % p) {
            return 0;
        }
    }

    long r = 1;
    for (; a >= p || b >= p; a /= p, b /= p) {
        r = (r + modbinom(a % p, b % p, p)) % p;
    }

    if (b > a - b) {
        b = a - b;
    }

    long d = 1;
    for (long i = 0; i < b; i++) {
        r = (r * (a - i)) % p;
        d = (d * (i + 1)) % p;
    }

    return (r * modinv(d, p)) % p;
}

int main(void) {
    freopen("jap2.in", "r", stdin);
    freopen("jap2.out", "w", stdout);

    int q;
    long p;

    scanf("%ld %d", &p, &q);

    for (int i = 0; i < q; i++) {
        long a, b;
        scanf("%ld %ld", &a, &b);
        printf("%ld\n", modbinom(a, b, p));
    }

    return 0;
}
