#include "8-1stuff.h"
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

int main(){
    int numOfVert, // aka N
        numOfEdges; // aka M
    if (inputCheck(&numOfVert, &numOfEdges) == INPUT_ERROR) return 0;

    if (numOfVert == 0){
        printf("no spanning tree");
        return 0;
    }


    int* graph = (int*)calloc(MAX*MAX, sizeof(int));
    if (graph == NULL) return 0;
    if (fillGraph(graph, numOfEdges, numOfVert) != SUCCESS){
        free(graph);
        return 0;
    }

    int* parent = (int*)calloc(numOfVert, sizeof(int));
    if (parent == NULL){
        free(graph);
        return 0;
    }
    if (prim(graph, parent, numOfVert) == 1){
        printf("no spanning tree");
        free(parent);
        free(graph);
        return 0;
    }

    for (int i = 1; i < numOfVert; ++i)
        if (parent[i] != NO_WAY)
            printf("%d %d\n", i + 1, parent[i] + 1);
    free(parent);
    free(graph);
    return 0;
}

