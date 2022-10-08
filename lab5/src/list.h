#ifndef LAB5_LIST_H
#define LAB5_LIST_H

typedef struct Tree Tree;
typedef struct List* ListPtr;

ListPtr createList();
int isEmptyList(ListPtr list);
void pushList(Tree* tree, int frequency, ListPtr* list);
void pushPriorityList(Tree* tree, int frequency, ListPtr* list);
Tree* popList (ListPtr* list);
void destroyList(ListPtr* list);

int listFreq(ListPtr list);
ListPtr listNext(ListPtr list);
Tree* listTree(ListPtr list);

#endif // LAB5_LIST_H
