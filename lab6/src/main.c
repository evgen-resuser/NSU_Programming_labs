#define _CRT_SECURE_NO_WARNINGS

#include "avlTree.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    unsigned int numberOfNodes;
    if (scanf("%u", &numberOfNodes) == EOF){
        printf("bad input");
        return 0;
    }

    tree* mainTree = (tree*)malloc(sizeof(tree)*numberOfNodes);
    if (mainTree == NULL){
        free(mainTree);
        return 1;
    }

    int value;
    tree* head = NULL;

    for(unsigned int i = 0; i < numberOfNodes; ++i){
        if (scanf("%d", &value) == EOF) {
            printf("bad input");
            free(mainTree);
            return 1;
        }
        head = addNodeToTree(head, &mainTree[i], value);
    }

    printf("%hhu", getHeight(head));
    free(mainTree);
    return 0;
}
