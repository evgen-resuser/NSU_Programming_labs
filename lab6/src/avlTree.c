#include "avlTree.h"
#include <stdio.h>

typedef unsigned char unchar;

int heightDifference(tree* node){
    return (getHeight(node->rightLeaf) - getHeight(node->leftLeaf));
}

unchar getHeight(tree* node){
    if (node) return node->height; else return 0;
}

static void fix(tree* node){
    unchar hLeft = getHeight(node->leftLeaf);
    unchar hRight = getHeight(node->rightLeaf);
    if (hLeft > hRight) node->height = 1 + hLeft; else node->height = 1 + hRight;
}

tree* rotate(tree* node, enum directions direction) {
    tree *node1 = NULL;

    if (direction == RIGHT){
        node1 = node->leftLeaf;
        node->leftLeaf = node1->rightLeaf;
        node1->rightLeaf = node;
    }

    if (direction == LEFT){
        node1 = node->rightLeaf;
        node->rightLeaf = node1->leftLeaf;
        node1->leftLeaf = node;
    }

    fix(node);
    fix(node1);
    return node1;
}

tree* balance(tree* node){
    fix(node);
    switch (heightDifference(node)) {
        case 2:
            if (heightDifference(node -> rightLeaf) < 0) node->rightLeaf = rotate(node->rightLeaf, RIGHT);
            return rotate(node, LEFT);
        case -2:
            if (heightDifference(node -> leftLeaf) > 0) node -> leftLeaf = rotate(node->leftLeaf, LEFT);
            return rotate(node, RIGHT);
        default: return node;
    }
}

tree* createNode(tree* newNode, int val){
    if(newNode) {
        newNode->value = val;
        newNode->leftLeaf = NULL;
        newNode->rightLeaf = NULL;
        newNode->height = 1;
    }
    return newNode;
}

tree* addNodeToTree(tree* parentNode, tree* node, int val){
    if (!parentNode) return createNode(node, val);
    if (val < parentNode->value){
        parentNode->leftLeaf = addNodeToTree(parentNode->leftLeaf, node, val);
    } else parentNode->rightLeaf = addNodeToTree(parentNode->rightLeaf, node, val);
    return balance(parentNode);
}
