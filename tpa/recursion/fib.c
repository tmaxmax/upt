#include <stdio.h>
#include <string.h>

char *fib_string(char *dst, int n, const char *a, const char *b) {
    switch (n) {
    case 2:
        return strcat(dst, b);
    case 1:
        return strcat(dst, a);
    default:
        break;
    }

    if (n <= 0) {
        return NULL;
    }

    fib_string(dst, n - 2, a, b);
    fib_string(dst, n - 1, a, b);

    return dst;
}

int main(void) {
    char buf[256] = {'\0'};

    printf("%s\n", fib_string(buf, 4, "aa", "b")); // expected: baab

    return 0;
}
