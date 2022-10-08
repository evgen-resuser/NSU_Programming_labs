#ifndef LAB8_0_KRUSKAL_STUFF_H
#define LAB8_0_KRUSKAL_STUFF_H

typedef struct node {
    int from;
    int to;
    int len;
} node;

typedef enum errors{
    SUCCESS = 0,
    FAILURE = 1,
    INPUT_ERROR = 3,
    MEMORY_ERROR = 2,
} errors;

int compareFunction(const void* x, const void* y);
errors doKruskal(node* graph, int numOfVert, int numOfEdges);

#endif //LAB8_0_KRUSKAL_STUFF_H
