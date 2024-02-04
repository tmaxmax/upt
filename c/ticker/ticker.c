#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

#include "ticker.h"

struct _Ticker {
    pthread_mutex_t mu;
    pthread_cond_t cv;
    bool done;
};

Ticker *ticker_new(void) {
    Ticker *t = malloc(sizeof *t);
    if (t == NULL) {
        return NULL;
    }

    if (pthread_mutex_init(&t->mu, NULL) != 0) {
        free(t);
        return NULL;
    }

    if (pthread_cond_init(&t->cv, NULL) != 0) {
        pthread_mutex_destroy(&t->mu);
        free(t);
        return NULL;
    }

    return t;
}

void ticker_free(Ticker *d) {
    pthread_mutex_destroy(&d->mu);
    pthread_cond_destroy(&d->cv);
    free(d);
}

struct ThreadData {
    Ticker *ticker;
    TickerCallback callback;
    void *callback_data;
};

static int64_t timespec_to_ns(const struct timespec *ts) {
    return ts->tv_sec * 1000000000 + ts->tv_nsec;
}

static void add_ns_to_timespec(struct timespec *out, const struct timespec *ts,
                               int64_t ns) {
    out->tv_sec = ts->tv_sec + ns / 1000000000;
    out->tv_nsec = ts->tv_nsec + ns % 1000000000;
    if (out->tv_nsec >= 1000000000) {
        out->tv_sec++;
        out->tv_nsec -= 1000000000;
    }
}

static void *thread(void *arg) {
    struct ThreadData *d = arg;
    struct timespec start, now, wait_until;

    pthread_mutex_lock(&d->ticker->mu);
    d->ticker->done = false;
    pthread_mutex_unlock(&d->ticker->mu);

    for (bool done = false; !done;) {
        timespec_get(&now, TIME_UTC);
        if (start.tv_sec == 0) {
            start = now;
        }

        int64_t next_interval = d->callback(
            d->callback_data, timespec_to_ns(&start), timespec_to_ns(&now));
        if (next_interval <= 0) {
            pthread_mutex_lock(&d->ticker->mu);
            d->ticker->done = true;
            pthread_mutex_unlock(&d->ticker->mu);
            break;
        }

        add_ns_to_timespec(&wait_until, &now, next_interval);

        pthread_mutex_lock(&d->ticker->mu);
        for (int ret = 0; !d->ticker->done && ret == 0;) {
            ret = pthread_cond_timedwait(&d->ticker->cv, &d->ticker->mu,
                                         &wait_until);
        }
        done = d->ticker->done;
        pthread_mutex_unlock(&d->ticker->mu);
    }

    free(d);

    return NULL;
}

int ticker_start(Ticker *t, TickerCallback callback, void *callback_data) {
    // Must malloc because the thread outlives the caller's stack frame.
    struct ThreadData *data = malloc(sizeof *data);
    data->ticker = t;
    data->callback = callback;
    data->callback_data = callback_data;

    pthread_t tid;
    const int ret = pthread_create(&tid, NULL, thread, data);
    if (ret != 0) {
        return ret;
    }

    return pthread_detach(tid);
}

void ticker_stop(Ticker *t) {
    pthread_mutex_lock(&t->mu);
    if (!t->done) {
        t->done = true;
        pthread_cond_signal(&t->cv);
    }
    pthread_mutex_unlock(&t->mu);
}
