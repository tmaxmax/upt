#include <stdio.h>

typedef struct {
    int x, y;
} Snowflake;

typedef struct {
    int edges[200];
    int num_edges;
} Vertex;

Snowflake determine_snowflake(Vertex *graph, int num_vertices) {
    int num_single_edged = 0;

    for (int i = 0; i < num_vertices; i++) {
        Vertex *v = &graph[i];
        if (v->num_edges > 1) {
            continue;
        }

        num_single_edged++;
    }

    int num_intermediary_edged = 0;
    for (int i = 0; i < num_vertices; i++) {
        Vertex *v = &graph[i];

        int num_edges = v->num_edges;
        if (num_edges == 1) {
            continue;
        }

        for (int j = 0; j < v->num_edges; j++) {
            num_edges -= (graph[v->edges[j]].num_edges == 1);
        }

        num_intermediary_edged += (num_edges == 1);
    }

    Snowflake r = {num_intermediary_edged,
                   num_single_edged / num_intermediary_edged};

    return r;
}

int main(void) {
    int n;
    scanf("%d", &n);

    Vertex graph[200];

    for (int i = 0; i < n; i++) {
        int n, m;
        scanf("%d %d", &n, &m);

        for (int j = 0; j < m; j++) {
            int from, to;
            scanf("%d %d", &from, &to);

            if (j == 0) {
                graph[from - 1].num_edges = 0;
                graph[to - 1].num_edges = 0;
            }
            graph[from - 1].edges[graph[from - 1].num_edges++] = to - 1;
            graph[to - 1].edges[graph[to - 1].num_edges++] = from - 1;
        }

        Snowflake r = determine_snowflake(graph, n);

        printf("%d %d\n", r.x, r.y);
    }

    return 0;
}
