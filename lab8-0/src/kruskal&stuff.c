#include "kruskal&stuff.h"

#include "stdio.h"
#include "stdlib.h"

#define NO_WAY -1

static void swap(int* a, int*b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

static int findSet(int* parent, const int vert){
    if (vert == parent[vert]) return vert;
    return parent[vert] = findSet(parent, parent[vert]);
}

static errors uniteSets(int* parent, int* rank, int leadFrom, int leadTo){
    if (leadFrom == leadTo) return FAILURE;

    if (rank[leadFrom] < rank[leadTo]) swap(&leadFrom, &leadTo);

    parent[leadTo] = leadFrom;

    if (rank[leadFrom] == rank[leadTo]) (rank[leadFrom])++;

    return SUCCESS;
}


int compareFunction(const void* x, const void* y){
    return ((node*)x)->len - ((node*)y)->len;
}

errors doKruskal(node* graph, const int numOfVert, const int numOfEdges){
    if (numOfVert == 0) return FAILURE;

    qsort(graph, numOfEdges, sizeof(node), compareFunction);

    int* parent = (int*)malloc(numOfVert*sizeof(int));
    if (parent == NULL) {
        free(parent);
        return MEMORY_ERROR;
    }
    for (int i = 0; i < numOfVert; ++i){
        parent[i] = i;
    }

    int* rank = (int*)calloc(numOfVert, sizeof(int));
    char* visited = (char*)calloc(numOfVert, sizeof(char));
    if (rank == NULL || visited == NULL) {
        free(rank);
        free(visited);
        return MEMORY_ERROR;
    }

    for(int i = 0; i < numOfEdges; ++i){
        int from = graph[i].from-1;
        int to = graph[i].to-1;

        int leadFrom = findSet(parent, from);
        int leadTo = findSet(parent, to);

        if (uniteSets(parent, rank, leadFrom, leadTo) == SUCCESS){
            visited[from] = 1;
            visited[to] = 1;
        } else graph[i].len = NO_WAY;
    }

    for(int i = 0; i < numOfVert; ++i){
        if (!visited[i]){
            free(rank);
            free(visited);
            free(parent);
            return FAILURE;
        }
    }

    free(rank);
    free(visited);
    free(parent);
    return SUCCESS;
}
