#include <assert.h>
#include <stdio.h>

typedef struct Node {
    int a, b;
    int max;
    struct Node *smaller, *greater;
} Node;

#define MAX_INPUT_SIZE 100000

Node pool[2 * MAX_INPUT_SIZE];
int pool_next;

Node *node_get(int a, int b, int max) {
    Node *n = &pool[pool_next++];
    n->a = a;
    n->b = b;
    n->max = max;
    return n;
}

Node *tree_build(const int *v, int start, int end) {
    if (start == end) {
        return node_get(start, end, v[start]);
    }

    int mid = start + (end - start) / 2;

    Node *smaller = tree_build(v, start, mid);
    Node *greater = tree_build(v, mid + 1, end);

    int max = smaller->max;
    if (greater->max > max) {
        max = greater->max;
    }

    Node *n = node_get(start, end, max);
    n->smaller = smaller;
    n->greater = greater;

    return n;
}

Node *tree_update(Node *root, int pos, int val) {
    if (root == NULL) {
        return node_get(pos, pos, val);
    }

    if (root->a == root->b) {
        root->max = val;
        return root;
    }

    if (pos <= root->smaller->b) {
        root->smaller = tree_update(root->smaller, pos, val);
    } else {
        root->greater = tree_update(root->greater, pos, val);
    }

    root->max = root->smaller->max;
    if (root->greater->max > root->max) {
        root->max = root->greater->max;
    }

    return root;
}

const int *tree_query(Node *root, int a, int b) {
    if (root == NULL) {
        return NULL;
    }

    if (a <= root->a && root->b <= b) {
        return &root->max;
    }

    const int *found_smaller = NULL;
    if (a <= root->smaller->b) {
        found_smaller = tree_query(root->smaller, a, b);
    }

    const int *found_greater = NULL;
    if (b >= root->greater->a) {
        found_greater = tree_query(root->greater, a, b);
    }

    if (found_smaller == NULL) {
        return found_greater;
    }
    if (found_greater == NULL) {
        return found_smaller;
    }
    if (*found_smaller > *found_greater) {
        return found_smaller;
    }
    return found_greater;
}

int main(void) {
    freopen("arbint.in", "r", stdin);
    freopen("arbint.out", "w", stdout);

    int n, m;
    scanf("%d %d", &n, &m);

    int v[MAX_INPUT_SIZE];
    for (int i = 0; i < n; i++) {
        scanf("%d", v + i);
    }

    Node *root = tree_build(v, 0, n - 1);

    int kind, a, b;
    for (int i = 0; i < m; i++) {
        scanf("%d %d %d", &kind, &a, &b);

        if (kind == 0) {
            printf("%d\n", *tree_query(root, a - 1, b - 1));
        } else if (kind == 1) {
            root = tree_update(root, a - 1, b);
        }
    }

    return 0;
}
