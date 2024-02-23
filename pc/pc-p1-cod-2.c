#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *county;
    char *place;
    int num_animals;
} Location;

bool loc_from_line(char *line, Location *out) {
    static const char *const DELIM = ";";

    char *token = strtok(line, DELIM);
    if (token == NULL) {
        return false;
    }
    out->county = strdup(token);

    if ((token = strtok(NULL, DELIM)) == NULL) {
        free(out->county);
        return false;
    }
    out->place = strdup(token);

    if ((token = strtok(NULL, DELIM)) == NULL) {
        free(out->county);
        free(out->place);
        return false;
    }
    if ((token = strtok(NULL, DELIM)) == NULL) {
        free(out->county);
        free(out->place);
        return false;
    }

    const long in_num = strtol(token, NULL, 10);
    if (errno != 0 || in_num < 0 || in_num > INT_MAX) {
        free(out->county);
        free(out->place);
        return false;
    }
    out->num_animals = atoi(token);

    return true;
}

void loc_write(const Location *l, FILE *out) {
    fprintf(out, "%s - %s - %d", l->county, l->place, l->num_animals);
}

void loc_free(const Location *l) {
    free(l->county);
    free(l->place);
}

typedef struct {
    Location *ptr;
    size_t len, cap;
} Repository;

Repository repo_new(void) { return (Repository){NULL, 0, 0}; }

void repo_add(Repository *r, const Location *l) {
    if (r->len == r->cap) {
        r->cap = r->cap == 0 ? 4 : r->cap / 2 * 3;
        r->ptr = realloc(r->ptr, r->cap * sizeof *r->ptr);
        assert(r->ptr != NULL);
    }

    r->ptr[r->len++] = *l;
}

void repo_write(const Repository *r, FILE *out) {
    for (const Location *l = r->ptr, *end = r->ptr + r->len; l < end; l++) {
        loc_write(l, out);
        fputc('\n', out);
    }
}

void repo_free(const Repository *r) {
    for (const Location *l = r->ptr, *end = r->ptr + r->len; l < end; l++) {
        loc_free(l);
    }
    free(r->ptr);
}

typedef int (*RepositorySorter)(const void *, const void *);

int repo_sorter_by_num_animals(const void *a, const void *b) {
    return ((const Location *)a)->num_animals -
           ((const Location *)b)->num_animals;
}

void repo_sort_by(Repository *r, RepositorySorter sorter) {
    qsort(r->ptr, r->len, sizeof *r, sorter);
}

bool read_line(FILE *in, char **buf, size_t *cap) {
    char tmp[1024];
    size_t len = 0;
    bool has_read = false;

    while (fgets(tmp, sizeof tmp, in)) {
        has_read = true;

        const size_t pos_newline = strcspn(tmp, "\r\n");
        const bool has_newline =
            tmp[pos_newline] == '\r' || tmp[pos_newline] == '\n';

        str_concat(buf, tmp, pos_newline);
        if (has_newline) {
            break;
        }
    }

    if (len + 1 > *cap) {
        str_concat(buf, "", 1);
        return buf->ptr;
    }

    return NULL;
}

size_t read_locations_csv(FILE *in, Repository *out) {
    Location input_location;
    str buf = str_new();
    size_t num_line = 0;

    for (char *line; (line = read_line(in, &buf)) != NULL;) {
        num_line++;
        if (num_line <= 2) { // first two lines are headers, skip them
            continue;
        }

        if (!loc_from_line(line, &input_location)) {
            str_free(&buf);
            repo_free(out);

            return num_line;
        }

        repo_add(out, &input_location);
    }

    str_free(&buf);

    return 0;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr,
                "CLI args should provide the input and the output files\n");
        return 1;
    }

    FILE *in = fopen(argv[1], "r");
    if (in == NULL) {
        perror("failed to open input file: ");
        return 1;
    }

    FILE *out = fopen(argv[2], "w");
    if (out == NULL) {
        perror("failed to open/create output file: ");

        (void)fclose(in); // there's not much we can do about this

        return 1;
    }

    Repository r = repo_new();

    const size_t invalid_line_num = read_locations_csv(in, &r);
    if (invalid_line_num != 0) {
        fprintf(stderr, "invalid location at line %zu\n", invalid_line_num);

        (void)fclose(in);
        (void)fclose(out);

        return 1;
    }

    repo_sort_by(&r, repo_sorter_by_num_animals);
    repo_write(&r, out);

    (void)fclose(in);
    (void)fclose(out);
    repo_free(&r);

    return 0;
}
