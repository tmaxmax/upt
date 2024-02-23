#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "polish/polish.h"

// To compile (from this directory):
// $ gcc -I../c/ -o polish.out polish.c polish/compute.c \
//   polish/internal/common.c polish/parse.c

int main(void) {
    char input[1024];
    char output[sizeof input];

    if (fgets(input, sizeof input, stdin) == NULL) {
        return 1;
    }
    input[strcspn(input, "\n")] = '\0';

    ParseResult pr = polish_parse(input, output, sizeof output);

    switch (pr.kind) {
    case ParseResultKindSuccess:
        break;
    case ParseResultKindUnbalanced:
        fprintf(stderr, "polish: unbalanced parentheses\n");
        break;
    case ParseResultKindTooNested:
        fprintf(stderr, "polish: expression too nested\n");
        break;
    case ParseResultKindInvalidToken:
        fprintf(stderr, "polish: invalid token '%c' (code %d)\n", pr.token,
                (int)pr.token);
        break;
    case ParseResultKindTooLong:
        // TooLong cannot occur because we allocate enough space for the output.
        assert(!"the output buffer is not big enough");
    }

    if (pr.kind != ParseResultKindSuccess) {
        return 1;
    }

    Variables vars = polish_vars_new();
    char name = 'a';
    for (double d; name <= 'z' && fscanf(stdin, " %lf", &d) == 1; name++) {
        if (!polish_vars_set(&vars, name, d)) {
            fprintf(stderr, "polish: invalid value for variable '%c'\n", name);
            return 1;
        }
    }

    ComputeResult cr = polish_compute(output, &vars);
    switch (cr.kind) {
    case ComputeResultKindSuccess:
        printf("%s = %g\n", output, cr.result);
        break;
    case ComputeResultKindIncomplete:
        fprintf(stderr, "polish: incomplete expression\n");
        break;
    case ComputeResultKindInvalidToken:
        fprintf(stderr, "polish: invalid token '%c' (code %d)\n", cr.token,
                (int)cr.token);
        break;
    case ComputeResultKindUndefined:
        fprintf(stderr, "polish: undefined variable '%c'\n", cr.variable_name);
        break;
    case ComputeResultKindTooComplex:
        fprintf(stderr, "polish: expression too complex\n");
        break;
    }

    if (cr.kind != ComputeResultKindSuccess) {
        return 1;
    }

    return 0;
}
