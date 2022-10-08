#include "kruskal&stuff.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX 5000
#define NO_WAY -1

int inputCheck(int* numOfVert, int* numOfEdges){
    if (scanf("%d", numOfVert) == EOF)
    { printf("bad number of lines"); return INPUT_ERROR; }
    if (scanf("%d", numOfEdges) == EOF)
    { printf("bad number of lines"); return INPUT_ERROR; }

    if (*numOfVert < 0 || *numOfVert > MAX){
        printf("bad number of vertices");
        return INPUT_ERROR;
    }
    if (*numOfEdges < 0 || *numOfEdges > (*numOfVert * (*numOfVert - 1) / 2)){
        printf("bad number of edges");
        return INPUT_ERROR;
    }
    return SUCCESS;
}


char fillGraph(node* graph, const int numOfEdges, const int numOfVert){
    int from, to, len;
    for (int i = 0; i != numOfEdges; ++i){
        if(scanf("%d %d %d", &from, &to, &len) == EOF){
            printf("bad number of lines");
            free(graph);
            return FAILURE;
        }
        if (from > numOfVert || from < 0 || to > numOfVert || to < 0){
            printf("bad vertex");
            free(graph);
            return FAILURE;
        }
        if (len < 0 || len > INT_MAX) {
            printf("bad length");
            free(graph);
            return FAILURE;
        }
        graph[i].from = from;
        graph[i].to = to;
        graph[i].len = len;
    }
    return SUCCESS;
}


int main() {
    int numOfVert, // aka N
        numOfEdges; // aka M
    if (inputCheck(&numOfVert, &numOfEdges) == INPUT_ERROR) return SUCCESS;
    if(numOfVert == 1 && numOfEdges == 0) return SUCCESS;

    node* graph = (node*)malloc(numOfEdges*sizeof(node));
    if (graph == NULL) {
        free(graph);
        return MEMORY_ERROR;
    }

    if(fillGraph(graph, numOfEdges, numOfVert) != SUCCESS) return 0;

    switch (doKruskal(graph, numOfVert,numOfEdges)){
        case FAILURE:
            printf("no spanning tree");
            free(graph);
            return 0;
        case MEMORY_ERROR:
            free(graph);
            return 0;
        case SUCCESS:
            break;
        case INPUT_ERROR:
            break;
    }

    for (int i = 0; i != numOfEdges; i++)
        if (graph[i].len != NO_WAY)
            printf("%d %d\n", graph[i].from, graph[i].to);


    free(graph);
    return 0;
}
