#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE 1000
#define NUM_HEADER_FIELDS 6
#define FIELDS_SEP ';'

typedef struct {
    const char *name;
    int index;
} HeaderField;

char *read_line(char *buf, size_t size_with_nul, FILE *in) {
    char *s = fgets(buf, size_with_nul, in);
    if (s == NULL) {
        return NULL;
    }

    size_t newline_index = strcspn(s, "\r\n");
    if (s[newline_index] == '\0') {
        return NULL;
    }

    s[newline_index] = '\0';

    return s;
}

typedef struct {
    const char *str;
    size_t len;
} sv;

#define SV_P "%.*s"
#define SV_PARGS(s) (int)((s).len), (s).str

sv sv_new_nul(const char *str) { return (sv){str, strlen(str)}; }

int sv_atoi(sv s) {
    if (s.len == 0) {
        return 0;
    }

    int res = 0;
    bool is_negative = s.str[0] == '-';

    for (size_t i = is_negative || s.str[0] == '+'; i < s.len; i++) {
        if (!isdigit(s.str[i])) {
            return 0;
        }

        res *= 10;
        res += s.str[i] - '0';
    }

    return (is_negative ? -res : res);
}

size_t split_string(const char *s, const char sep, sv *dst,
                    size_t max_parts_count) {
    const char sep_str[] = {sep, '\0'};

    size_t num_parts = 0;

    const char *start_part = s;
    for (const char *end_part;
         (end_part = strpbrk(start_part, sep_str)) != NULL;
         num_parts++, start_part = end_part + 1) {
        if (num_parts > max_parts_count) {
            return num_parts;
        }

        sv part = {start_part, end_part - start_part};
        dst[num_parts] = part;
    }

    if (num_parts < max_parts_count) {
        dst[num_parts] = (sv){start_part, strlen(start_part)};
    }

    return num_parts + 1;
}

static const char *FIELD_COUNTY = "Judet"; // 5 - 5 = 0; 0 % 6 = 0
static const char *FIELD_NUM_BENEFICIARIES =
    "Beneficiari platiti"; // 19 - 5 = 14; 14 % 6 = 2
static const char *FIELD_TOTAL_PAID_SUM =
    "Suma totala platita drepturi curente"; // 36 - 5 = 31; 31 % 6 = 1
static const char *FIELD_POPULATION =
    "Populatie"; // 9 - 5 = 4; 4 % 6 = 4; 4 - 1 = 3
static const char *FIELD_OTHER_PAYMENTS =
    "Alte plati"; // 10 - 5 = 5; 5 % 6 = 5; 5 - 1 = 4

#define NUM_EXPECTED_HEADER_FIELDS 5

size_t field_perfect_hash(const sv field_name) {
    size_t len = field_name.len;
    len -= 5;
    len %= 6;
    return len - (len > 2);
}

HeaderField *get_header_field(HeaderField field_map[NUM_EXPECTED_HEADER_FIELDS],
                              const sv field_name) {
    size_t hash = field_perfect_hash(field_name);
    if (hash > NUM_EXPECTED_HEADER_FIELDS) {
        return NULL;
    }

    return field_map + hash;
}

HeaderField *
get_header_field_nul(HeaderField field_map[NUM_EXPECTED_HEADER_FIELDS],
                     const char *s) {
    return get_header_field(field_map, sv_new_nul(s));
}

bool find_expected_fields(const char *header, const char sep,
                          HeaderField dst[NUM_EXPECTED_HEADER_FIELDS]) {
    *get_header_field_nul(dst, FIELD_COUNTY) = (HeaderField){FIELD_COUNTY, -1};
    *get_header_field_nul(dst, FIELD_NUM_BENEFICIARIES) =
        (HeaderField){FIELD_NUM_BENEFICIARIES, -1};
    *get_header_field_nul(dst, FIELD_TOTAL_PAID_SUM) =
        (HeaderField){FIELD_TOTAL_PAID_SUM, -1};
    *get_header_field_nul(dst, FIELD_POPULATION) =
        (HeaderField){FIELD_POPULATION, -1};
    *get_header_field_nul(dst, FIELD_OTHER_PAYMENTS) =
        (HeaderField){FIELD_OTHER_PAYMENTS, -1};

    size_t num_found = 0;

    sv fields[NUM_HEADER_FIELDS];
    const size_t num_input_fields =
        split_string(header, sep, fields, NUM_HEADER_FIELDS);
    if (num_input_fields != NUM_HEADER_FIELDS) {
        return false;
    }

    for (size_t i = 0; i < num_input_fields; i++) {
        HeaderField *maybe = get_header_field(dst, fields[i]);
        if (maybe == NULL ||
            strncmp(maybe->name, fields[i].str, fields[i].len)) {
            continue;
        }

        if (maybe->index != -1) {
            return false;
        }

        maybe->index = i;
        num_found++;
    }

    return num_found == NUM_EXPECTED_HEADER_FIELDS;
}

typedef struct {
    char name[20];
    int population;
    int num_beneficiaries;
    int total_paid_sum;
    int other_payments;
} County;

bool new_county(const sv fields[NUM_HEADER_FIELDS],
                HeaderField fields_map[NUM_EXPECTED_HEADER_FIELDS],
                County *out) {
    const sv name =
        fields[get_header_field_nul(fields_map, FIELD_COUNTY)->index];
    if (name.len >= sizeof out->name) {
        return false;
    }
    strncpy(out->name, name.str, name.len);

    const sv num_beneficiaries =
        fields[get_header_field_nul(fields_map, FIELD_NUM_BENEFICIARIES)
                   ->index];
    out->num_beneficiaries = sv_atoi(num_beneficiaries);

    const sv total_paid_sum =
        fields[get_header_field_nul(fields_map, FIELD_TOTAL_PAID_SUM)->index];
    out->total_paid_sum = sv_atoi(total_paid_sum);

    const sv population =
        fields[get_header_field_nul(fields_map, FIELD_POPULATION)->index];
    out->population = sv_atoi(population);

    const sv other_payments =
        fields[get_header_field_nul(fields_map, FIELD_OTHER_PAYMENTS)->index];
    out->other_payments = sv_atoi(other_payments);

    return true;
}

int county_sorter_by_name(const void *a, const void *b) {
    return strcmp(((const County *)a)->name, ((const County *)b)->name);
}

#define COUNTY_SORTER_BY_INT(field)                                            \
    int county_sorter_by_##field(const void *a, const void *b) {               \
        return ((const County *)a)->field - ((const County *)b)->field;        \
    }

COUNTY_SORTER_BY_INT(total_paid_sum)
COUNTY_SORTER_BY_INT(population)
COUNTY_SORTER_BY_INT(other_payments)
COUNTY_SORTER_BY_INT(num_beneficiaries)

void sort_counties(County *counties, size_t count,
                   int (*sorter)(const void *a, const void *b)) {
    qsort(counties, count, sizeof *counties, sorter);
}

void sort_counties_by_name(County *counties, size_t count) {
    sort_counties(counties, count, county_sorter_by_name);
}

void sort_counties_by_total_paid_sum(County *counties, size_t count) {
    sort_counties(counties, count, county_sorter_by_total_paid_sum);
}

int main(int argc, char **argv) {
    int ret_code = 0;

    if (argc < 2) {
        fprintf(stderr, "open_data: no input file given\n");
        return 1;
    }

    FILE *input = fopen(argv[1], "r"), *output = NULL;
    if (input == NULL) {
        perror("open_data: failed to open input file");
        return 1;
    }

    char line[MAX_LINE_SIZE];

    const char *header = read_line(line, sizeof line, input);
    if (header == NULL) {
        if (errno == 0) {
            fprintf(stderr,
                    "open_data: header unexpectedly long (must be under %zu "
                    "bytes long)\n",
                    sizeof line);
        } else {
            perror("open_data: failed to read header line");
        }

        goto CLEANUP_ERROR;
    }

    HeaderField fields_map[NUM_EXPECTED_HEADER_FIELDS];
    if (!find_expected_fields(header, FIELDS_SEP, fields_map)) {
        fprintf(stderr, "open_data: invalid header\n");
        goto CLEANUP_ERROR;
    }

    County *counties = NULL;
    size_t counties_len = 0, counties_cap = 0;
    sv fields[NUM_HEADER_FIELDS];

    while (read_line(line, sizeof line, input)) {
        const size_t num_fields =
            split_string(line, FIELDS_SEP, fields, NUM_HEADER_FIELDS);
        if (num_fields != NUM_HEADER_FIELDS) {
            fprintf(stderr,
                    "open_data: invalid field count (got %zu, expected %d)\n",
                    num_fields, NUM_HEADER_FIELDS);
            goto CLEANUP_ERROR;
        }

        if (counties_len == counties_cap) {
            counties_cap = (counties_cap / 2) * 3;
            if (counties_cap == 0) {
                counties_cap = 4;
            }

            counties = realloc(counties, counties_cap * sizeof *counties);
        }

        if (!new_county(fields, fields_map, counties + counties_len)) {
            const sv name =
                fields[get_header_field_nul(fields_map, FIELD_COUNTY)->index];
            fprintf(stderr, "open_data: county name \"" SV_P "\" too long\n",
                    SV_PARGS(name));
            goto CLEANUP_ERROR;
        }

        counties_len++;
    }

    if (errno != 0) {
        perror("open_data: failed to read file");
        goto CLEANUP_ERROR;
    }

    output = fopen("rezultat.txt", "w");
    if (output == NULL) {
        perror("open_data: failed to open output file");
        goto CLEANUP_ERROR;
    }

    sort_counties(counties, counties_len, county_sorter_by_population);
    fprintf(output, "%s\n", counties[counties_len - 1].name);
    sort_counties(counties, counties_len, county_sorter_by_other_payments);
    fprintf(output, "%s\n", counties[counties_len - 1].name);

    goto CLEANUP;

CLEANUP_ERROR:;
    ret_code = 1;
CLEANUP:;
    int input_close = 0, output_close = 0;
    if (input != NULL) {
        input_close = fclose(input);
    }
    if (output != NULL) {
        output_close = fclose(output);
    }

    if (input_close || output_close) {
        perror("open_data: I/O error");
    }

    return ret_code;
}
