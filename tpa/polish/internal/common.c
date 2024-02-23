#include "common.h"

int precedence(char operator) {
    switch (operator) {
    case '*':
    case '/':
        return 2;
    case '+':
    case '-':
        return 1;
    default:
        return -1;
    }
}

bool is_valid_name(char name) {
    // Keep in sync with the function inside compute.c.
    return ('a' <= name && name <= 'z') || ('A' <= name && name <= 'Z');
}
