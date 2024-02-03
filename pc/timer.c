#include <assert.h>
#include <errno.h>
#include <poll.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

bool try_read_line(int timeout_ms, char **buf, size_t *cap) {
    const int fd = STDIN_FILENO;

    struct pollfd pfd = {.fd = fd, .events = POLLIN};
    if (poll(&pfd, 1, timeout_ms) != 1 || !(pfd.revents & POLLIN)) {
        return false;
    }

    size_t len = 0;

    char input[256];
    ssize_t num_read;

    do {
        num_read = read(fd, input, sizeof input);
        if (num_read <= 0) {
            break;
        }

        const size_t pos_newline = strcspn(input, "\r\n");
        const bool has_newline =
            input[pos_newline] == '\r' || input[pos_newline] == '\n';

        if (len + pos_newline > *cap) {
            *cap = *cap == 0 ? pos_newline : *cap * 2;
            *buf = realloc(*buf, *cap);
        }

        memcpy(*buf + len, input, pos_newline);
        len += pos_newline;

        if (has_newline) {
            break;
        }
    } while ((size_t)num_read == sizeof input);

    if (len + 1 > *cap) {
        *cap = len + 1;
        *buf = realloc(*buf, *cap);
    }

    (*buf)[len] = '\0';

    return true;
}

int rand_num_in_range(int min, int max) {
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

enum Operation {
    OP_ADD,
    OP_SUB,
};

int do_operation(enum Operation op, int a, int b) {
    switch (op) {
    case OP_ADD:
        return a + b;
    case OP_SUB:
        return a - b;
    default:
        assert(!"unreachable");
    }
}

const char *operation_to_string(enum Operation op) {
    switch (op) {
    case OP_ADD:
        return "+";
    case OP_SUB:
        return "-";
    default:
        assert(!"unreachable");
    }
}

int main(int argc, char **argv) {
    if (!isatty(STDIN_FILENO)) {
        fprintf(stderr, "application should be opened in a terminal\n");
        return 1;
    }

    int timeout_ms = 3000;
    if (argc > 1) {
        const int parsed = atoi(argv[1]);
        if (parsed > 0) {
            timeout_ms = parsed * 1000;
        }
    }

    printf("How much is it? You have %d seconds!\n\n", timeout_ms);

    srand(time(NULL));

    int min = 1, max = 100;
    int num_attempts = 0;

    char *input = NULL;
    size_t input_cap = 0;

    while (true) {
        const enum Operation op = rand_num_in_range(OP_ADD, OP_SUB);
        const int a = rand_num_in_range(min, max);
        const int b = rand_num_in_range(min, max);
        const int res = do_operation(op, a, b);

        printf("%d %s %d = ", a, operation_to_string(op), b);
        fflush(stdout);

        if (!try_read_line(timeout_ms, &input, &input_cap)) {
            printf("\nTime's up!\n");
            break;
        }

        const int parsed = strtol(input, NULL, 10);
        if (errno != 0) {
            perror("invalid input");
            free(input);
            return 1;
        }

        if (parsed != res) {
            printf("Incorrect! The correct answer is %d\n", res);
            break;
        }

        printf("Correct!\n");
        num_attempts++;
        max = max / 4 * 5;
    }

    printf("You answered %d questions. Good job!\n", num_attempts);

    free(input);

    return 0;
}
