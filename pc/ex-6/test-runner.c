#include <stdio.h>
#include <sys/resource.h>
#include <sys/time.h>

extern int run(void);

int main(void) {
    struct timeval start, end, duration;
    gettimeofday(&start, NULL);
    int ret = run();
    gettimeofday(&end, NULL);

    if (ret) {
        return ret;
    }

    timersub(&end, &start, &duration);
    long duration_us = duration.tv_sec * 1000000 + duration.tv_usec;

    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage)) {
        return 2;
    }

    fprintf(stderr, "%ld %ld\n", duration_us, usage.ru_maxrss);

    return 0;
}
