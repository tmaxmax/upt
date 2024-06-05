#include <stdio.h>

typedef struct Node {
    int val, count;
    struct Node *left, *right;
} Node;

#define MAX_INPUT_SIZE 100000

Node pool[MAX_INPUT_SIZE];
long pool_next;

Node *node_get(int val, int count) {
    Node *n = &pool[pool_next++];
    n->val = val;
    n->count = count;
    return n;
}

int main(void) {
    freopen("inv.in", "r", stdin);
    freopen("inv.out", "w", stdout);

    int n;
    scanf("%d", &n);

    Node *root = NULL;
    long total = 0;
    const long mod = 9917;

    for (int i = 0; i < n; i++) {
        int v;
        scanf("%d", &v);

        Node *p = node_get(v, 0);
        if (root == NULL) {
            root = p;
            continue;
        }

        Node *q = root;
        while (1) {
            Node **child = NULL;
            if (p->val < q->val) {
                total = (total + 1 + q->count) % mod;
                child = &q->left;
            } else {
                q->count = (q->count + 1) % mod;
                child = &q->right;
            }
            if (*child != NULL) {
                q = *child;
            } else {
                *child = p;
                break;
            }
        }
    }

    printf("%ld\n", total);

    return 0;
}
