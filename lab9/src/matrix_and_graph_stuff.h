#ifndef LAB9_MATRIX_AND_GRAPH_STUFF_H
#define LAB9_MATRIX_AND_GRAPH_STUFF_H

int sumArithProgression(int begin,int end,int count);
int* createMatrix(int count);
int getMtrxIndx(int row,int column,int size);
void addValue(int row, int column, int size, int length, int* matrix);
int getValue(int row, int column, int size, const int* matrix);
void destroyMatrix(int* matrix);


typedef struct Graph Graph;
struct Graph{
    int vertCount;
    int* matrix;
};

Graph createGraph(int vertexCount);
void push(int begin, int end, int length, Graph* graph);
int getEdgeLen(int begin, int end, Graph* graph);
void destroyGraph(Graph* graph);

#endif //LAB9_MATRIX_AND_GRAPH_STUFF_H
