#include "tab.h"

const char *PROGRAM_NAME = "detab";

void process(FILE *in, FILE *out, int tab_size) {
    for (int c; (c = fgetc(in)) != EOF;) {
        if (c != '\t') {
            fputc(c, out);
            continue;
        }

        for (int i = 0; i < tab_size; i++) {
            fputc(' ', out);
        }
    }
}
