#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tab.h"

typedef struct {
    int tab_size;
    const char *input_name;
    const char *output_name;
} Args;

int parse_args(int argc, char **argv, Args *out) {
    static struct option opts[] = {
        {"tab-size", required_argument, NULL, 't'},
        {"input-file", required_argument, NULL, 'i'},
        {"output-file", required_argument, NULL, 'o'},
        {NULL, 0, NULL, 0},
    };

    while (true) {
        int opt = getopt_long_only(argc, argv, "t:i:o:", opts, NULL);

        switch (opt) {
        case 't':
            if (optarg) {
                out->tab_size = (int)strtol(optarg, NULL, 10);
            }
            break;
        case 'i':
            out->input_name = optarg;
            break;
        case 'o':
            out->output_name = optarg;
            break;
        case '?':
            return -1;
        default:
            return 0;
        }
    }
}

bool is_default_file(const char *name) {
    return name == NULL || strcmp(name, "-") == 0;
}

FILE *open_file(const char *name, const char *mode, FILE *implicit,
                bool *must_close) {
    if (is_default_file(name)) {
        *must_close = false;
        return implicit;
    }

    FILE *f = fopen(name, mode);
    *must_close = f != NULL;
    return f;
}

int main(int argc, char **argv) {
    Args args = {.tab_size = 8, .input_name = NULL, .output_name = NULL};
    if (parse_args(argc, argv, &args)) {
        return 1;
    }

    fprintf(stderr, "%s\n  Tab size: %d\n  Input: %s\n  Output: %s\n",
            PROGRAM_NAME, args.tab_size,
            is_default_file(args.input_name) ? "stdin" : args.input_name,
            is_default_file(args.output_name) ? "stdout" : args.output_name);

    bool in_must_close;
    FILE *in = open_file(args.input_name, "r", stdin, &in_must_close);
    if (in == NULL) {
        fprintf(stderr, "%s: failed to open input \"%s\"\n", PROGRAM_NAME,
                args.input_name);
        return 1;
    }

    bool out_must_close;
    FILE *out = open_file(args.output_name, "w+", stdout, &out_must_close);
    if (out == NULL) {
        fprintf(stderr, "%s: failed to open output \"%s\"\n", PROGRAM_NAME,
                args.output_name);
        if (in_must_close) {
            (void)fclose(in);
        }

        return 1;
    }

    process(in, out, args.tab_size);

    if (in_must_close) {
        (void)fclose(in);
    }

    if (out_must_close) {
        (void)fclose(out);
    }

    return 0;
}
