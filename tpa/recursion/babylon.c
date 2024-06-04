#include <math.h>
#include <stdio.h>

double sqrt_babylon(double x, double a) {
    double an = 0.5 * (a + x / a);
    if (fabs(a - an) < 1e-6) {
        return an;
    }
    return sqrt_babylon(x, an);
}

int main(void) {
    printf("%f\n", sqrt_babylon(2.0, 1.0));

    return 0;
}
