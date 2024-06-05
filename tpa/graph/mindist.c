#include <stdio.h>
#include <stdlib.h>

#define MAX_INPUT_SIZE 1000

#define CELL_BLOCKED -1
#define CELL_UNSET -2

typedef struct {
    int i, j;
} Position;

int main(void) {
    const Position directions[] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

    int n, m, k;
    scanf("%d %d %d", &n, &m, &k);

    int *graph = malloc((unsigned)(n * n) * sizeof *graph);
    for (int i = 0; i < n * n; i++) {
        graph[i] = CELL_UNSET;
    }

    for (int i = 0; i < m; i++) {
        int p, q;
        scanf("%d %d", &p, &q);

        graph[(p - 1) * n + q - 1] = CELL_BLOCKED;
    }

    Position *dsts = malloc((unsigned)(n * n) * sizeof *dsts);
    int dsts_end = 0, dsts_start = 0;

    for (int i = 0; i < k; i++) {
        int p, q;
        scanf("%d %d", &p, &q);

        dsts[dsts_end].i = p - 1;
        dsts[dsts_end++].j = q - 1;

        graph[(p - 1) * n + q - 1] = 0;
    }

    while (dsts_end > dsts_start) {
        Position pd = dsts[dsts_start];

        for (unsigned id = 0; id < sizeof directions / sizeof *directions;
             id++) {
            Position d = directions[id];
            Position p = {pd.i + d.i, pd.j + d.j};

            if (0 <= p.i && p.i <= n && 0 <= p.j && p.j <= n &&
                graph[p.i * n + p.j] != CELL_BLOCKED &&
                (graph[p.i * n + p.j] == CELL_UNSET ||
                 graph[pd.i * n + pd.j] + 1 < graph[p.i * n + p.j])) {
                dsts[dsts_end++] = p;
                graph[p.i * n + p.j] = graph[pd.i * n + pd.j] + 1;
            }
        }

        dsts_start++;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (graph[i * n + j] == CELL_UNSET) {
                graph[i * n + j] = CELL_BLOCKED;
            }
            printf("%d ", graph[i * n + j]);
        }
        printf("\n");
    }

    free(graph);
    free(dsts);

    return 0;
}
