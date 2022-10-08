#include "matrix_and_graph_stuff.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX 5000
typedef unsigned long long ullong;

//-------------------------------------------

int findNearest(const int numOfVert, const ullong* ways, const char* visited){
    int minNum = 0;
    ullong min = ULLONG_MAX;

    for (int i = 0; i < numOfVert; ++i)
        if (visited[i] == 0 && ways[i] <= min){
            min = ways[i];
            minNum = i+1;
        }
    return minNum;
}

void changeLen (const int vertex, ullong* ways, const char* visited, Graph graph){
    for (int i = 0; i < graph.vertCount; ++i){
        ullong len = getEdgeLen(vertex, i+1, &graph);
        if (len > 0 && visited[i] == 0 && ways[i] > len + ways[vertex-1])
            ways[i] = len+ways[vertex-1];
    }
}

ullong* doDijkstra(const int start, Graph* graph){
    char* visited = calloc(graph->vertCount, sizeof(*visited));
    if (visited == NULL) return NULL;
    ullong* ways = calloc(graph->vertCount, sizeof(*ways));
    if (ways == NULL) {
      free(visited);
      return NULL;}

    for (int i = 0; i < graph->vertCount; ++i){
        if (i != start - 1)
            ways[i] = ULLONG_MAX;
    }
    for (int i = 0; i < graph->vertCount; ++i){
        int vertex = findNearest(graph->vertCount, ways, visited);
        visited[vertex-1] = 1;

        changeLen(vertex, ways, visited, *graph);
    }

    free(visited);
    return ways;
}

//-----------------------------------------------------------
int inputCheck(int* numOfVert, int* numOfEdges, int* start, int* finish){
    if (scanf("%d", numOfVert) == EOF)
    { printf("bad number of lines"); return 1; }
    if (scanf("%d %d", start, finish) == EOF)
    { printf("bad number of lines"); return 1;}

    if (scanf("%d", numOfEdges) == EOF)
    { printf("bad number of lines"); return 1; }


    if (*start > *numOfVert || *start < 1 || *finish > *numOfVert || *finish < 1 ){
        printf("bad vertex");
        return 1;
    }
    if (*numOfVert < 0 || *numOfVert > MAX){
        printf("bad number of vertices");
        return 1;
    }
    if (*numOfEdges < 0 || *numOfEdges > (*numOfVert * (*numOfVert - 1) / 2)){
        printf("bad number of edges");
        return 1;
    }
    return 0;
}

//---------------------------------

char checkForOverflow(const int vertNum, const int size, ullong* ways){
    if (ways[vertNum-1] <= INT_MAX){
        return 0;
    }

    int count = 0;
    for (int i = 0; i < size; ++i){
        if (i != vertNum - 1 && ways[i] != ULLONG_MAX && ways[i] >= INT_MAX){
            ++count;
            if(count == 2)
                return 1;
        }
    }
    return 0;
}

int findPrevVert(const int vertNum, const ullong* ways, Graph* graph){
    for (int i = 0; i < graph->vertCount; ++i){
        const ullong len = getEdgeLen(vertNum, i+1, graph);
        if (len > 0 && ways[vertNum-1] == len + ways[i])
            return i+1;
    }
    return 0;
}

int main(){
    int numOfVert, // aka N
        numOfEdges; // aka M
    int start, finish;

    if (inputCheck(&numOfVert, &numOfEdges, &start, &finish)){ //== 1
        return 0;
    }

    Graph graph = createGraph(numOfVert);
    if (graph.matrix == NULL) return 0;

    for (int i = 0; i != numOfEdges; ++i){
        int from, to, len;
        if (scanf("%d %d %d", &from, &to, &len) == EOF){
            printf("bad number of lines");
            destroyGraph(&graph);
            return 0;
        }
        if (from < 0 || from > numOfVert || to < 0 || to > numOfVert){
            printf("bad vertex");
            destroyGraph(&graph);
            return 0;
        }
        if (len < 0 || len > INT_MAX){
            printf("bad length");
            destroyGraph(&graph);
            return 0;
        }
        push(from, to, len, &graph);
    }

    ullong* ways = doDijkstra(start, &graph);
    if (ways == NULL) return 0;

    //------ answer output ------

    for (int i = 0; i < numOfVert; ++i){
        if (ways[i] == ULLONG_MAX)
            printf("oo ");
        else if (ways[i] > INT_MAX)
            printf("INT_MAX+ ");
        else printf("%d ", (int)ways[i]);
    }
    printf("\n");
    if (ways[finish-1] == ULLONG_MAX){
        printf("no path");
        free(ways);
        destroyGraph(&graph);
        return 0;
    }
    else if (checkForOverflow(finish, graph.vertCount, ways)){
        printf("overflow");
        free(ways);
        destroyGraph(&graph);
        return 0;
    }

    printf("%d ", finish);

    while (ways[finish-1] != 0){
        finish = findPrevVert(finish, ways, &graph);
        if (finish == 0){
            free(ways);
            destroyGraph(&graph);
        }

        printf("%d ", finish);
    }

    free(ways);
    destroyGraph(&graph);

    return 0;
}
