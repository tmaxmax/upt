#include <assert.h>
#include <ctype.h>
#include <stdio.h>

#include "tab.h"

const char *PROGRAM_NAME = "entab";

#define SPACE ' '
#define TAB '\t'

static void fputc_n(const char c, const int n, FILE *out) {
    for (int i = 0; i < n; i++) {
        (void)fputc(c, out);
    }
}

static int copy_upto_space(FILE *in, FILE *out) {
    int cnt = 0;
    int c;

    while ((c = fgetc(in)) != EOF && c != SPACE) {
        (void)fputc(c, out);

        if (isspace(c)) {
            cnt = 0;
        } else {
            cnt++;
        }
    }

    assert(ungetc(c, in) == c);

    return cnt;
}

static int consume_space(FILE *in, char *after_expected) {
    int cnt = 0;
    int c;

    while ((c = fgetc(in)) != EOF && c == SPACE) {
        cnt++;
    }

    *after_expected = (char)c;
    assert(ungetc(c, in) == c);

    return cnt;
}

void process(FILE *in, FILE *out, int tab_size) {
    while (!feof(in)) {
        const int num_non_space = copy_upto_space(in, out) % tab_size;

        char after_space;
        const int num_space = consume_space(in, &after_space);

        const int blank_span = num_non_space + num_space;
        const int num_tabs = blank_span / tab_size;
        if (num_tabs == 0) {
            fputc_n(SPACE, num_space, out);
            continue;
        }

        if (num_space == 1 && after_space != TAB) {
            (void)fputc(SPACE, out);
            continue;
        }

        fputc_n(TAB, num_tabs, out);
        fputc_n(SPACE, num_tabs * tab_size - blank_span, out);
    }
}
