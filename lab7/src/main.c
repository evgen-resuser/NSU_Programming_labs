#define _CRT_SECURE_NO_WARNINGS
#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>

#define MAX 2000

enum colors{
    WHITE, //not visited
    GRAY, //WIP
    BLACK, //visited and checked
};

void freeAll(int* answer, char* checked, char* matrix){
    free(answer); free(checked); free(matrix);
}

errors inputCheck(int* numOfVert, int* numOfEdges){
    if (scanf("%d", numOfVert) == EOF)
    { printf("bad number of lines"); return INPUT_ERROR; }
    if (scanf("%d", numOfEdges) == EOF)
    { printf("bad number of lines"); return INPUT_ERROR; }

    if (*numOfVert < 0 || *numOfVert > MAX){
        printf("bad number of vertices");
        return INPUT_ERROR;
    }
    if (*numOfEdges < 0 || *numOfEdges > (*numOfVert * (*numOfVert + 1) / 2)){
        printf("bad number of edges");
        return INPUT_ERROR;
    }
    return SUCCESS;
}

errors doDFS(char* matrix, char* used, int cur, int* output, int* cur_out, const int n)
{

    if (used[cur] == BLACK)
        return SUCCESS;

    if (used[cur] == GRAY)
        return IMPOSSIBLE_TO_SORT;

    used[cur] = GRAY;

    for (int i = 0; i < n; ++i)
        if (getMatrixValue(matrix, cur + 1, i + 1) != 0)
            if (doDFS(matrix, used, i, output, cur_out, n) != SUCCESS)
                return IMPOSSIBLE_TO_SORT;

    used[cur] = BLACK;
    output[*cur_out] = (int)cur+1;
    ++(*cur_out);
    return SUCCESS;
}

errors startSort(char* matrix, char* checked, int* answer, const int numOfVert, int* currentPos){
    for(int i = 0; i != numOfVert; i++) checked[i] = WHITE;

    for (int i = 0; i < numOfVert; ++i)
        if (doDFS(matrix, checked, i, answer, currentPos, numOfVert) != SUCCESS){
            printf("impossible to sort");
            freeAll(answer, checked, matrix);
            return IMPOSSIBLE_TO_SORT;
        }
    return SUCCESS;
}


int main(){
    int numOfVert, //aka N
        numOfEdges;//aka M

    if (inputCheck(&numOfVert, &numOfEdges) == INPUT_ERROR) return 0;

    char* matrix = createMatrix();
    if (fillMatrix(matrix, numOfVert, numOfEdges) == INPUT_ERROR) return 0;


    char* checked = malloc(sizeof(char)*numOfVert);
    int* answer = malloc(sizeof(int)*numOfVert);
    int currentPos = 0;
    if (checked == NULL || answer == NULL){
        freeAll(answer, checked, matrix);
        return 0;
    }

    if (startSort(matrix, checked, answer, numOfVert, &currentPos) != SUCCESS){
        return 0;
    }

    for (int i = currentPos -1; i >= 0; --i) printf("%d ", answer[i]);

    freeAll(answer, checked, matrix);
    return 0;
}
