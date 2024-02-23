#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct {
    Node *start;
    Node *end;
} List;

typedef struct {
    Node *a;
    Node *b;
} RemovedNodes;

void InitList(List *margini) {
    margini->start = NULL;
    margini->end = NULL;
}

void AddListData(List *margini, int value_to_add) {
    Node *vector = malloc(sizeof(Node));
    if (margini->start == NULL) {
        margini->start = vector;
        margini->end = vector;
        vector->value = value_to_add;
        vector->next = NULL;
    } else {
        margini->end->next = vector;
        margini->end = vector;
        vector->value = value_to_add;
        vector->next = NULL;
    }
}

void FreeList(List margini) {
    Node *vector = margini.start;
    Node *next_vector = vector->next;
    for (; vector != NULL; vector = next_vector) {
        next_vector = vector->next;
        free(vector);
    }
}

RemovedNodes FindSameValues(List la, List lb) {
    Node *na = la.start, *nb = lb.start;
    RemovedNodes removed;
    for (; na != NULL; na = na->next) {
        for (Node *b = nb; b != NULL; b = b->next) {
            if (na->value == b->value) {
                removed.a = na;
                removed.b = b;
                return removed;
            }
        }
    }
    removed.a = NULL;
    removed.b = NULL;
    return removed;
}

void DeleteValue(List *margini, Node *to_delete) {
    Node *elem_ant = NULL;
    for (List *i = margini->start; i != list_to_delete; i = i->next_item) {
        elem_ant = i;
    }

    if (elem_ant == NULL) {
        margini->start = list_to_delete->next_item;
        free(list_to_delete);
    } else if (list_to_delete == margini->end) {
        free(list_to_delete);
        margini->end = elem_ant;
    } else {
        elem_ant->next_item = list_to_delete->next_item;
        free(list_to_delete);
    }
}

void RemoveSameValues(RemovedNodes values, List *list_a, List *list_b) {
    DeleteValue(list_a, values.a);
    DeleteValue(list_b, values.b);
}

void RemoveAndAddToC(List *list_c, List *list_a, List *list_b) {
    InitList(list_c);
    RemovedNodes values = FindSameValues(*list_a, *list_b);
    while (values.a != NULL) {
        AddListData(list_c, values.a->value);
        RemoveSameValues(values, list_a, list_b);
        values = FindSameValues(*list_a, *list_b);
    }
}

void ShowListValues(List list_to_show) {
    for (List *iterator = list_to_show.start; iterator != NULL;
         iterator = iterator->next_item) {
        printf("%d ", iterator->value);
    }
    printf("\n");
}

int main(void) {
    List list_a, list_b, list_c;
    InitList(&list_a);
    InitList(&list_b);
    InitList(&list_c);
    int nr = 0;
    while (scanf("%d", &nr) == 1 && nr != 0) {
        AddListData(&list_a, nr);
    }

    while (scanf("%d", &nr) == 1 && nr != 0) {
        AddListData(&list_b, nr);
    }

    RemoveAndAddToC(&list_c, &list_a, &list_b);
    ShowListValues(list_c);
    ShowListValues(list_b);
    ShowListValues(list_a);
    FreeList(list_a);
    FreeList(list_b);
    FreeList(list_c);
    return 0;
}
