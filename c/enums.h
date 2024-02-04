#ifndef C_ENUMS_H
#define C_ENUMS_H

#define ENUM_VARIANTS(typ)                                                     \
    struct {                                                                   \
        const char *input;                                                     \
        typ output;                                                            \
    }

#define GEN_ENUM_PARSE(typ, name, variants)                                    \
    const char *name##_parse(const char *s, typ *out) {                        \
        for (size_t i = 0;                                                     \
             i < sizeof name##_variants / sizeof name##_variants[0]; i++) {    \
            const size_t len = strlen(name##_variants[i].input);               \
            if (strncasecmp(s, name##_variants[i].input, len) == 0) {          \
                *out = name##_variants[i].output;                              \
                return s + len;                                                \
            }                                                                  \
        }                                                                      \
        return NULL;                                                           \
    }

#define GEN_ENUM_TO_STRING(typ, name, variants)                                \
    const char *name##_to_string(typ e) {                                      \
        for (size_t i = 0;                                                     \
             i < sizeof name##_variants / sizeof name##_variants[0]; i++) {    \
            if (name##_variants[i].output == e) {                              \
                return name##_variants[i].input;                               \
            }                                                                  \
        }                                                                      \
        return NULL;                                                           \
    }

#endif
