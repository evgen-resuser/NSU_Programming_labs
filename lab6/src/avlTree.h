//
// Created by Evgen on 23.03.2022.
//

#ifndef LAB6_AVLTREE_H
#define LAB6_AVLTREE_H

typedef enum directions{
    LEFT = 'l',
    RIGHT = 'r',
} directions;

typedef struct node tree;
struct node{
    int value;
    unsigned char height;
    struct node* rightLeaf;
    struct node* leftLeaf;
};

int heightDifference(tree* node);
unsigned char getHeight(tree* node);

tree* rotate (tree* node, directions direction);
tree* balance (tree* node);
tree* createNode (tree* newNode, int val);
tree* addNodeToTree(tree* parentNode, tree* node, int val);


#endif //LAB6_AVLTREE_H
