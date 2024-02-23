#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>

#include "polish.h"

#include "internal/common.h"
#define StackValue double
#include "internal/stack.h"

Variables polish_vars_new(void) { return (Variables){{0}, 0}; }

static unsigned get_name_index(char name) {
    if ('a' <= name && name <= 'z') {
        return name - 'a' + 26;
    }
    return name - 'A';
}

bool polish_vars_set(Variables *v, char name, double value) {
    if (!is_valid_name(name) || !isfinite(value)) {
        return false;
    }

    const unsigned index = get_name_index(name);
    v->values[index] = value;
    v->is_set |= (1ULL << index);

    return true;
}

static const double *polish_vars_get(const Variables *v, char name) {
    assert(is_valid_name(name));
    const unsigned index = get_name_index(name);
    if ((v->is_set & (1ULL << index)) != 0) {
        return &v->values[index];
    }
    return NULL;
}

#define xstack_push(s, v)                                                      \
    do {                                                                       \
        if (!stack_push(s, v)) {                                               \
            return (ComputeResult){ComputeResultKindTooComplex, {0}};          \
        }                                                                      \
    } while (0)

static double do_operation(double a, double b, char op) {
    switch (op) {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        return a / b;
    default:
        assert(!"unexpected operator");
    }
}

ComputeResult polish_compute(const char *polish_form,
                             const Variables *variables) {
    assert(polish_form != NULL);
    assert(variables != NULL);

    double buf[128];
    Stack ops = stack_new(buf, sizeof buf / sizeof buf[0]);

    while (*polish_form != '\0') {
        char c = *polish_form++;

        if (isspace(c)) {
            continue;
        }

        if (is_valid_name(c)) {
            const double *value = polish_vars_get(variables, c);
            if (value == NULL) {
                return (ComputeResult){ComputeResultKindUndefined,
                                       {.variable_name = c}};
            }
            xstack_push(&ops, *value);
            continue;
        }

        const bool is_operator = precedence(c) != -1;
        if (is_operator) {
            double a, b;
            if (!stack_pop(&ops, &b)) {
                return (ComputeResult){ComputeResultKindIncomplete, {0}};
            }
            if (!stack_pop(&ops, &a)) {
                return (ComputeResult){ComputeResultKindIncomplete, {0}};
            }
            xstack_push(&ops, do_operation(a, b, c));
            continue;
        }

        return (ComputeResult){ComputeResultKindInvalidToken, {.token = c}};
    }

    double result;
    if (!stack_pop(&ops, &result) || stack_size(&ops) != 0) {
        return (ComputeResult){ComputeResultKindIncomplete, {0}};
    }

    return (ComputeResult){ComputeResultKindSuccess, {result}};
}
