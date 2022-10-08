#include "tree.h"

#include <stdio.h>
#include <stdlib.h>

struct Tree{
  char symbol;
  TreePtr left;
  TreePtr right;
};

int isEmpty(const TreePtr tree){
  return tree == NULL;
}

int isLeaf(const TreePtr tree){
  if (isEmpty(tree))
    return 0;
  else
    return isEmpty(tree->left) && isEmpty(tree->right);
}

TreePtr createTree(const char symbol, TreePtr left, TreePtr right){
    TreePtr aNew = malloc(sizeof(*aNew));
  if (aNew == NULL) return NULL;

  aNew->symbol = symbol;
  aNew->left = left;
  aNew->right = right;
  return aNew;
}

int countLeaves(const TreePtr tree){
  if (isEmpty(tree))
    return 0;
  else if(isLeaf(tree))
    return 1;
  return countLeaves(tree->left) + countLeaves(tree->right);
}

void treeCutter(TreePtr* tree){
  if (!isEmpty(*tree)){
    treeCutter(&(*tree)->right);
    treeCutter(&(*tree)->left);
    free(*tree);
  }
}

TreePtr leftTree(const TreePtr tree){
  return tree->left;
}

TreePtr rightTree(const TreePtr tree){
  return tree->right;
}

char symbolFromTree(const TreePtr tree){
  return tree->symbol;
}
