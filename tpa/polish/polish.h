#ifndef TPA_POLISH_POLISH_H
#define TPA_POLISH_POLISH_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum ParseResultKind {
    ParseResultKindSuccess,
    ParseResultKindTooLong,
    ParseResultKindTooNested,
    ParseResultKindUnbalanced,
    ParseResultKindInvalidToken,
};

typedef struct {
    enum ParseResultKind kind;
    char token;
} ParseResult;

ParseResult polish_parse(const char *infix_form, char *output,
                         const size_t max_len);

typedef struct {
    double values[52];
    uint64_t is_set;
} Variables;

Variables polish_vars_new(void);
bool polish_vars_set(Variables *v, char name, double value);

enum ComputeResultKind {
    ComputeResultKindSuccess,
    ComputeResultKindIncomplete,
    ComputeResultKindInvalidToken,
    ComputeResultKindUndefined,
    ComputeResultKindTooComplex
};

typedef struct {
    enum ComputeResultKind kind;
    union {
        double result;
        char token;
        char variable_name;
    };
} ComputeResult;

ComputeResult polish_compute(const char *polish_form,
                             const Variables *variables);

#endif
