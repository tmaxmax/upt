#ifndef C_TRY_READ_H
#define C_TRY_READ_H

#include <stddef.h>
#include <sys/types.h>

// Attempts to read a line of at most out_max_size chars from stdin
// before timeout_ms milliseconds have passed.
// If the line is too long, any characters beyond out_max_size are discarded.
// Return values:
//  - >0: number of characters read
//  - 0: EOF
//  - -1: timeout
//  - -2: line too long
//  - -3: error
ssize_t try_read_line(int timeout_ms, char *out, size_t out_max_size);

#ifdef TRY_READ_IMPLEMENTATION

#include <poll.h>
#include <unistd.h>

ssize_t try_read_line(int timeout_ms, char *out, size_t out_max_size) {
    const int fd = STDIN_FILENO;
    struct pollfd pfd = {.fd = fd, .events = POLLIN};

    switch (poll(&pfd, 1, timeout_ms)) {
    case -1:
        return -3;
    case 0:
        return -1;
    default:
        if (!(pfd.revents & POLLIN)) {
            return -3;
        }
        break;
    }

    ssize_t num_read = read(fd, out, out_max_size);
    if (num_read == 0) {
        return 0;
    }

    if (out[num_read - 1] == '\n') {
        out[num_read - 1] = '\0';
        return num_read;
    }

    // drain the input, so that the next call to try_read_line will
    // read a new line, not what is left from the old line.
    while ((num_read = read(fd, out, out_max_size)) == out_max_size)
        ;

    return -2;
}

#endif

#endif
