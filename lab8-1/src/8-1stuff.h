#ifndef LAB8_1_8_1STUFF_H
#define LAB8_1_8_1STUFF_H

typedef enum errors{
    INPUT_ERROR = -1,
    SUCCESS = 0,
    FAILURE = 1,
} errors;

void addValue(int* graph, int from, int to, int value);
int getValue(int* graph, int from, int to);
errors fillGraph(int* graph, int numOfEdges, int numOfVert);
errors prim(int* graph, int* parent, int numOfVert);


#endif //LAB8_1_8_1STUFF_H
