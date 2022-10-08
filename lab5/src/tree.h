#ifndef LAB5_TREE_H
#define LAB5_TREE_H

typedef struct Tree* TreePtr;

int isEmpty(TreePtr tree);
int isLeaf(TreePtr tree);
TreePtr createTree(char symbol, TreePtr left, TreePtr right);
int countLeaves(TreePtr tree);
void treeCutter(TreePtr* tree);

TreePtr leftTree(TreePtr tree);
TreePtr rightTree(TreePtr right);
char symbolFromTree(TreePtr tree);

#endif // LAB5_TREE_H
