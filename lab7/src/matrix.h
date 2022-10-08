#ifndef LAB7_MATRIX_H
#define LAB7_MATRIX_H

typedef enum errors{
    SUCCESS = 0,
    IMPOSSIBLE_TO_SORT = -1,
    INPUT_ERROR = -10,
} errors;

char* createMatrix();
void addMatrixValue(char* matrix,int pos1, int pos2);
errors fillMatrix(char* matrix, int numOfVert, int numOfEdges);
char getMatrixValue(char* matrix, int pos1, int pos2);


#endif //LAB7_MATRIX_H
