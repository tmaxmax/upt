#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/time.h>

extern "C" int solve_backpack(const int *weights, const int *profits, int n,
                              int max_weight);

int main(void) {
    int n, max_weight;
    scanf("%d %d", &n, &max_weight);

    int *weights = (int *)malloc((unsigned)n * sizeof *weights);
    int *profits = (int *)malloc((unsigned)n * sizeof *profits);

    for (int i = 0; i < n; i++) {
        scanf("%d %d", weights + i, profits + i);
    }

    struct timeval start, end, duration;
    gettimeofday(&start, NULL);
    int solution = solve_backpack(weights, profits, n, max_weight);
    gettimeofday(&end, NULL);

    timersub(&end, &start, &duration);
    long duration_us = duration.tv_sec * 1000000 + duration.tv_usec;

    printf("%d %d %d %ld\n", n, max_weight, solution, duration_us);

    return 0;
}
