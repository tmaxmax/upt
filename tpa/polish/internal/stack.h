#ifndef TPA_POLISH_STACK_H
#define TPA_POLISH_STACK_H

#include <stdbool.h>
#include <stddef.h>

#ifndef StackValue
#error "StackValue must be defined before including stack.h"
#endif

typedef struct {
    size_t top, max;
    StackValue *data;
} Stack;

static Stack stack_new(StackValue *buffer, size_t max_size) {
    return (Stack){0, max_size, buffer};
}

static size_t stack_size(const Stack *s) { return s->top; }

static bool stack_peek(const Stack *s, StackValue *value) {
    if (s->top == 0) {
        return false;
    }
    *value = s->data[s->top - 1];
    return true;
}

static bool stack_push(Stack *s, StackValue value) {
    if (s->top == s->max) {
        return false;
    }
    s->data[s->top++] = value;
    return true;
}

static bool stack_pop(Stack *s, StackValue *value) {
    bool res = stack_peek(s, value);
    s->top -= res;
    return res;
}

#endif
