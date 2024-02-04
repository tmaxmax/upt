#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

#include "ticker.h"

struct Ticker {
    pthread_mutex_t mu;
    pthread_cond_t cv;
    bool done;
};

struct Ticker *ticker_new(void) {
    struct Ticker *t = malloc(sizeof *t);
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

void ticker_free(struct Ticker *d) {
    pthread_mutex_destroy(&d->mu);
    pthread_cond_destroy(&d->cv);
    free(d);
}

struct ThreadData {
    struct Ticker *ticker;
    TickerCallback callback;
    void *callback_data;
};

static void *thread(void *arg) {
    struct ThreadData *d = arg;
    struct timespec wait_until;
    bool done = false;

    pthread_mutex_lock(&d->ticker->mu);
    d->ticker->done = false;
    pthread_mutex_unlock(&d->ticker->mu);

    for (int i = 1; !done; i++) {
        d->callback(d->callback_data, i);

        timespec_get(&wait_until, TIME_UTC);
        wait_until.tv_sec += 1;

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

int ticker_start(struct Ticker *t, TickerCallback callback,
                 void *callback_data) {
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

void ticker_stop(struct Ticker *t) {
    pthread_mutex_lock(&t->mu);
    if (!t->done) {
        t->done = true;
        pthread_cond_signal(&t->cv);
    }
    pthread_mutex_unlock(&t->mu);
}
