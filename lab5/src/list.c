#include "list.h"
#include "tree.h"

#include <stdio.h>
#include <stdlib.h>

struct List {
  int frequency;
  TreePtr tree;
  ListPtr next;
};

ListPtr createList(){
  return NULL;
}

int isEmptyList(const ListPtr list) {
  return list == NULL;
}

void pushList(const TreePtr tree, const int frequency, ListPtr* list){
    ListPtr aNew = malloc(sizeof(*aNew));
  if (aNew == NULL)
      return;

  aNew->frequency = frequency;
  aNew->tree = tree;
  aNew->next = *list;
  *list = aNew;
}

void pushPriorityList(TreePtr tree, const int frequency, ListPtr* list){
  if (isEmptyList(*list) || (*list)->frequency >= frequency)
    pushList(tree, frequency, list);
  else pushPriorityList(tree, frequency, &(*list)->next);
}

TreePtr popList (ListPtr* list){
  if(isEmptyList(*list)) return NULL;

  TreePtr removeTree = (*list)->tree;
  ListPtr removeList = *list;
  *list = (*list)->next;
  free(removeList);
  return removeTree;
}

void destroyList(ListPtr* list){
  if (!isEmptyList(*list)){
    destroyList(&(*list)->next);
    treeCutter(&(*list)->tree);
    free(*list);
  }
}

int listFreq(const ListPtr list){
  return list->frequency;
}

ListPtr listNext(const ListPtr list){
  return list->next;
}

TreePtr listTree(const ListPtr list){
  return list->tree;
}
