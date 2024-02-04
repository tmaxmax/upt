#include <assert.h>
#include <poll.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

// Attempts to read a line of at most out_max_size chars from stdin
// before timeout_ms milliseconds have passed.
// If the line is too long, any characters beyond out_max_size are discarded.
// Return values:
//  - >0: number of characters read
//  - 0: EOF
//  - -1: timeout
//  - -2: line too long
//  - -3: error
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

struct TimerDisplay {
    pthread_mutex_t mu;
    pthread_cond_t cv;
    int num_seconds;
    bool done;
};

bool timer_display_init(struct TimerDisplay *d, int num_seconds) {
    if (pthread_mutex_init(&d->mu, NULL) != 0) {
        return false;
    }
    if (pthread_cond_init(&d->cv, NULL) != 0) {
        pthread_mutex_destroy(&d->mu);
        return false;
    }
    d->num_seconds = num_seconds;
    d->done = false;
    return true;
}

void timer_display_destroy(struct TimerDisplay *d) {
    pthread_mutex_destroy(&d->mu);
    pthread_cond_destroy(&d->cv);
}

void timer_display_stop(struct TimerDisplay *d) {
    pthread_mutex_lock(&d->mu);
    if (!d->done) {
        d->done = true;
        pthread_cond_signal(&d->cv);
    }
    pthread_mutex_unlock(&d->mu);
}

void *timer_display_thread(void *arg) {
    struct TimerDisplay *d = arg;
    struct timespec wait_until;

    printf("  :  \x1b[s");

    for (int i = d->num_seconds; i > 0; i--) {
        printf("\r%d : \x1b[u", i);
        fflush(stdout);
        printf("\x1b[s");

        timespec_get(&wait_until, TIME_UTC);
        wait_until.tv_sec += 1;

        pthread_mutex_lock(&d->mu);
        for (int ret = 0; !d->done && ret == 0;) {
            ret = pthread_cond_timedwait(&d->cv, &d->mu, &wait_until);
        }
        if (d->done) {
            break;
        }
        pthread_mutex_unlock(&d->mu);
    }

    return NULL;
}

int timer_display_start(struct TimerDisplay *d) {
    pthread_t tid;
    int ret;
    if ((ret = pthread_create(&tid, NULL, timer_display_thread, d)) != 0) {
        return ret;
    };

    assert(pthread_detach(tid) == 0);

    return 0;
}

ssize_t get_input_with_timer(int num_seconds, char *out, size_t out_max_size) {
    struct TimerDisplay d;
    if (!timer_display_init(&d, num_seconds)) {
        return -3;
    }
    if (timer_display_start(&d) != 0) {
        timer_display_destroy(&d);
        return -3;
    };

    ssize_t ret = try_read_line(num_seconds * 1000, out, out_max_size);
    timer_display_stop(&d);
    timer_display_destroy(&d);
    return ret;
}

int main(void) {
    char input[256];
    ssize_t ret = get_input_with_timer(5, input, sizeof input);
    switch (ret) {
    case 0:
        puts("EOF");
        break;
    case -1:
        puts("Timeout");
        break;
    case -2:
        puts("Line too long");
        break;
    case -3:
        puts("Error");
        break;
    default:
        printf("Read: %s\n", input);
        return 0;
    }

    return 1;
}
