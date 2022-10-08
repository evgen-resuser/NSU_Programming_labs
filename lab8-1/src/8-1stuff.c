#include "8-1stuff.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX 5000
#define NO_WAY -1

void addValue(int* graph, const int from, const int to, const int value){
    int pos = (to-1)*MAX+(from-1);
    graph[pos] = value;
}

int getValue(int* graph, const int from, const int to){
    return graph[(to-1)*MAX+(from-1)];
}
errors fillGraph(int* graph, const int numOfEdges, const int numOfVert){
    int from, to, len;
    for (int i = 0; i != numOfEdges; ++i){
        if(scanf("%d %d %d", &from, &to, &len) == EOF){
            printf("bad number of lines");
            return FAILURE;
        }
        if (from > numOfVert || from < 0 || to > numOfVert || to < 0){
            printf("bad vertex");
            return FAILURE;
        }
        if (len < 0 || len > INT_MAX) {
            printf("bad length");
            return FAILURE;
        }
        addValue(graph, from, to, len);
        addValue(graph, to, from, len);
    }
    return SUCCESS;
}

errors prim(int* graph, int* parent, const int numOfVert){
    long long* minEdge = (long long*)calloc(numOfVert, sizeof(long long));
    if (minEdge == NULL) return FAILURE;
    for (int i = 0; i < numOfVert; ++i){
        parent[i] = NO_WAY;
        minEdge[i] = LLONG_MAX;
    }
    minEdge[0] = 0;

    char* visited = (char*)calloc(numOfVert, sizeof(char));
    if (visited == NULL) {
        free(minEdge);
        return FAILURE;
    }

    for (int i = 0; i < numOfVert; ++i){
        int minIndx = -1;
        for (int j = 0; j < numOfVert; ++j)
            if (visited[j] != 1 && (minIndx == -1 || minEdge[j] < minEdge[minIndx]))
                minIndx = j;

        if (minEdge[minIndx] == LLONG_MAX){
            free(minEdge);
            free(visited);
            return FAILURE;
        }

        visited[minIndx] = 1;

        for (int j = 0; j < numOfVert; ++j){
            int temp = getValue(graph, minIndx + 1, j + 1);
            if (temp != 0 && visited[j] != 1 && minEdge[j] > temp) {
                minEdge[j] = temp;
                parent[j] = minIndx;
            }
        }
    }

    free(minEdge);
    free(visited);
    return SUCCESS;
}

