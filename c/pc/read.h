#ifndef PC_READ_H
#define PC_READ_H

#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>

#define PC_INTERNAL_READ_NUM_VEC_DECL(type, name)                              \
    extern int read_##name##s_eof(type *dst, size_t *count,                    \
                                  const size_t max_count, FILE *src);          \
    extern int read_##name##s_n(type *dst, const size_t n, FILE *src);

#ifdef PC_READ_IMPLEMENTATION
#define PC_INTERNAL_READ_NUM_VEC(type, name, format_string)                    \
    PC_INTERNAL_READ_NUM_VEC_DECL(type, name)                                  \
    int read_##name##s_eof(type *dst, size_t *count, const size_t max_count,   \
                           FILE *src) {                                        \
        int ret = 1;                                                           \
        for (*count = 0; *count < max_count; (*count)++) {                     \
            if ((ret = fscanf(src, "%" format_string " ", dst + *count)) !=    \
                1) {                                                           \
                break;                                                         \
            }                                                                  \
        }                                                                      \
        return ret;                                                            \
    }                                                                          \
    int read_##name##s_n(type *dst, const size_t n, FILE *src) {               \
        size_t i;                                                              \
        return read_##name##s_eof(dst, &i, n, src);                            \
    }
#else
#define PC_INTERNAL_READ_NUM_VEC(type, name, format_string)                    \
    PC_INTERNAL_READ_NUM_VEC_DECL(type, name)
#endif // PC_READ_IMPLEMENTATION

PC_INTERNAL_READ_NUM_VEC(uint8_t, u8, SCNu8)
PC_INTERNAL_READ_NUM_VEC(uint64_t, u64, SCNu64)

extern int read_to_eof_of(void *dst, const size_t sizeof_elem, size_t *count,
                          const size_t max_count, FILE *src,
                          int (*read_fn)(void *elem, FILE *src));
extern int read_n_of(void *dst, const size_t sizeof_elem, const size_t n,
                     FILE *src, int (*read_fn)(void *elem, FILE *src));
extern int read_scanf_ret(int ret, int expected);
extern char *read_line(FILE *in);

#ifdef PC_READ_IMPLEMENTATION
int read_to_eof_of(void *dst, const size_t sizeof_elem, size_t *count,
                   const size_t max_count, FILE *src,
                   int (*read_fn)(void *elem, FILE *src)) {
    int ret = 1;
    for (*count = 0; *count < max_count; (*count)++) {
        if ((ret = read_fn(dst + *count * sizeof_elem, src)) != 1) {
            break;
        }
    }
    return ret;
}
int read_n_of(void *dst, const size_t sizeof_elem, const size_t n, FILE *src,
              int (*read_fn)(void *elem, FILE *src)) {
    size_t i;
    return read_to_eof_of(dst, sizeof_elem, &i, n, src, read_fn);
}
int read_scanf_ret(int ret, int expected) {
    if (ret == expected) {
        return 1;
    } else if (ret == EOF) {
        return EOF;
    }
    return 0;
}

#ifndef PC_READ_LINE_BUFSIZE
#define PC_READ_LINE_BUFSIZE 256
#endif // PC_READ_LINE_BUFSIZE

char *read_line(FILE *in) {
    size_t len = 0, cap = 0;
    char *line = NULL;

    char *last_read_start;

    char buf[PC_READ_LINE_BUFSIZE];

    while (fgets(buf, sizeof buf, in) != NULL) {
        if (len + sizeof buf > cap) {
            cap = cap == 0 ? sizeof buf : (cap / 2 * 3);
            line = realloc(line, cap);
        }

        last_read_start = line + len;
        memcpy(line + len, buf, sizeof buf - 1);
        len += sizeof buf - 1;
    }

    if (line == NULL) {
        return NULL;
    }

    const size_t last_read_len = strcspn(last_read_start, "\r\n");
    last_read_start[last_read_len] = '\0';

    return line;
}
#endif // PC_READ_IMPLEMENTATION

#endif // PC_READ_H
