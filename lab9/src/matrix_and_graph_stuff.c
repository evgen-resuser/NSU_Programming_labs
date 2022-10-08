#include "matrix_and_graph_stuff.h"

#include <limits.h>
#include <stdlib.h>

int sumArithProgression(const int begin, const int end, const int count){
    return (begin + end) * count / 2;
}

//-----------------------------------------------------------
int* createMatrix(const int count){
    int* matrix = calloc(sumArithProgression(1, count, count), sizeof(*matrix));
    return matrix;
}

int getMtrxIndx(const int row, const int column, const int size){
    const int min = (row >= column) ? column : row;
    const int max = (row < column) ? column : row;

    return sumArithProgression(size, size - min + 1, min) + (max - min);
}

void addValue(const int row, const int column, const int size, const int length, int* matrix){
    const int index = getMtrxIndx(row, column, size);
    matrix[index] = length;
}

int getValue(const int row, const int column, const int size, const int* matrix){
    const int index = getMtrxIndx(row, column, size);
    return matrix[index];
}

void destroyMatrix(int* matrix){
    free(matrix);
}

//-----------------------------------------------------------

Graph createGraph(const int vertexCount){
    Graph graph = { vertexCount, NULL };
    graph.matrix = createMatrix(vertexCount);
    return graph;
}

void push(const int begin, const int end, const int length, Graph* graph){
    addValue(begin - 1, end - 1, graph->vertCount, length, graph->matrix);
}

int getEdgeLen(const int begin, const int end, Graph* graph){
    return getValue(begin - 1, end - 1, graph->vertCount, graph->matrix);
}

void destroyGraph(Graph* graph){
    destroyMatrix(graph->matrix);
}
