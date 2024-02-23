#include <assert.h>
#include <ctype.h>

#include "polish.h"

#include "internal/common.h"
#define StackValue char
#include "internal/stack.h"

#define xstack_push(s, v, err)                                                 \
    do {                                                                       \
        if (!stack_push(s, v)) {                                               \
            return (ParseResult){err, v};                                      \
        }                                                                      \
    } while (0)

ParseResult polish_parse(const char *infix_form, char *output,
                         const size_t max_len) {
    assert(infix_form != NULL);
    assert(output != NULL && max_len > 0);

    char ops_buf[128];
    Stack ops = stack_new(ops_buf, sizeof ops_buf);
    Stack res = stack_new(output, max_len);
    int nesting = 0;

    while (*infix_form != '\0') {
        char c = *infix_form++;

        if (isspace(c)) {
            continue;
        }

        if (is_valid_name(c)) {
            xstack_push(&res, c, ParseResultKindTooLong);
            continue;
        }

        if (c == '(') {
            nesting++;
            xstack_push(&ops, c, ParseResultKindTooNested);
            continue;
        }

        if (c == ')') {
            nesting--;
            for (char r; stack_pop(&ops, &r) && r != '(';) {
                xstack_push(&res, r, ParseResultKindTooLong);
            }
            continue;
        }

        const int prec = precedence(c);
        if (prec != -1) {
            for (char r; stack_peek(&ops, &r) && r != '(';) {
                if (precedence(r) < prec) {
                    break;
                }
                stack_pop(&ops, &r);
                xstack_push(&res, r, ParseResultKindTooLong);
            }
            xstack_push(&ops, c, ParseResultKindTooNested);
            continue;
        }

        return (ParseResult){ParseResultKindInvalidToken, c};
    }

    if (nesting != 0) {
        return (ParseResult){ParseResultKindUnbalanced, 0};
    }

    for (char r; stack_pop(&ops, &r);) {
        xstack_push(&res, r, ParseResultKindTooLong);
    }

    xstack_push(&res, '\0', ParseResultKindTooLong);

    return (ParseResult){ParseResultKindSuccess, 0};
}
