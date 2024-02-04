#ifndef C_TIMER_H
#define C_TIMER_H

#include <stdint.h>

// General purpose utility which allows doing something every second.
struct Ticker;

// A callback function that the Ticker calls when it starts and
// after the time duration provided by its last call.
// The start parameter is the time when the ticker started in nanoseconds.
// The now parameter is the current time in nanoseconds.
// The return value is the duration in nanoseconds after which the callback
// should be called again. The ticker stops if the return value is 0 or
// negative.
typedef int64_t (*TickerCallback)(void *data, int64_t start, int64_t now);

// Creates a Ticker. Must be freed with ticker_free.
struct Ticker *ticker_new(void);

// Starts a new thread on which the Ticker will run and the callback
// will be called. The callback_data is passed to the callback function.
// Returns 0 on success or a negative value on error.
int ticker_start(struct Ticker *timer, TickerCallback callback,
                 void *callback_data);

// Stops the ticker. It can be reused afterwards by calling ticker_start.
void ticker_stop(struct Ticker *timer);

void ticker_free(struct Ticker *timer);

#endif
