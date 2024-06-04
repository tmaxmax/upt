#include <math.h>
#include <stdio.h>

double func_root(double (*f)(double), double a, double b) {
    double c = a * 0.5 + b * 0.5;
    if (fabs(a - b) < 1e-6) {
        return c;
    }

    if (f(a) * f(c) < 0) {
        return func_root(f, a, c);
    }

    return func_root(f, c, b);
}

double f(double x) { return x * x - 2.0; }

int main(void) {
    printf("%f\n", func_root(f, 0, 2));

    return 0;
}
