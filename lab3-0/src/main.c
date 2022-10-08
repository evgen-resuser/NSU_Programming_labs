#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

void Swap(int* a, int* b){
    int buffer = *a;
    *a = *b;
    *b = buffer;
}

void Heap(int* array, const int len, int root){
    int largestValue = root;
    int left = 2*root+1;
    int right = 2*root+2;

    if(len > left && array[left] > array[largestValue])
        largestValue = left;

    if(len > right && array[right] > array[largestValue])
        largestValue = right;

    if(largestValue != root) {
        Swap(&array[largestValue], &array[root]);
        Heap(array, len, largestValue);
    }
}

void HeapSort(int* array, const int length){
    for (int i = (length / 2 - 1); i >= 0; --i) Heap(array, length, i);

    for (int i = (length-1); i >= 0; --i){
        Swap(&array[0], &array[i]);
        Heap(array, i, 0);
    }
}

int main() {
    int length;
    if (scanf("%d", &length) == EOF) return 0;

    int* array;
    array = (int*)malloc(length * sizeof(int));
    if(array == NULL) return 0;

    for(int i = 0; i < length; i++){
        if (scanf("%d", &array[i]) == EOF)
            return 0;
    }

    HeapSort(array, length);

    for (int i = 0; i < length; i++){
        printf("%d ", array[i]);
    }
    free(array);
    return 0;
}
