#include "matrix.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX 2000
#define INPUT_ERROR -10
#define SUCCESS 0
#define CHAR_SIZE 8

char* createMatrix(){
    char* matrix = calloc(MAX*MAX, sizeof(*matrix));
    return matrix;
}

void addMatrixValue(char* matrix, const int pos1, const int pos2){
    int indx = (pos2-1)*MAX/CHAR_SIZE + ((pos1 - 1)/CHAR_SIZE);
    char x = matrix[indx];
    int n = abs((pos1 - 1)%CHAR_SIZE-(CHAR_SIZE-1));
    x |= (1 << n);
    matrix[indx] = x;
}

errors fillMatrix(char* matrix, const int numOfVert, const int numOfEdges){
    int from, to;
    for (int i = 0; i != numOfEdges; ++i){
        if (scanf("%d %d", &from, &to) == EOF)
        {free(matrix); printf("bad number of lines"); return INPUT_ERROR;}

        if (from < 1 || from > numOfVert || to < 1 || to > numOfVert){
            printf("bad vertex");
            free(matrix);
            return INPUT_ERROR;
        }
        addMatrixValue(matrix, from, to);
    }
    return SUCCESS;
}

char getMatrixValue(char* matrix, const int pos1, const int pos2){
    int indx = (pos2-1)*MAX/CHAR_SIZE + ((pos1 - 1)/CHAR_SIZE);
    char x = matrix[indx];
    int n = abs((pos1 - 1)%CHAR_SIZE-(CHAR_SIZE-1));
    return x & (1 << n);
}
