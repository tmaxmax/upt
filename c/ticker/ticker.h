#ifndef C_TIMER_H
#define C_TIMER_H

// TODO: Extend functionality to support other time intervals
// and variable time intervals.

// General purpose utility which allows doing something every second.
struct Ticker;

// A callback function that the Ticker calls every time a second elapses.
typedef void (*TickerCallback)(void *data, int second);

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
