#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

void swap(int* a, int* b){
    int buffer = *a;
    *a = *b;
    *b = buffer;
}

void quickSort(int* array, int first, int last){
    if (first < last){
        int x = first;
        int y = last;
        int someValue = array[(first + last) / 2];
        while (x < y){

            while (array[x] < someValue) ++x;
            while (array[y] > someValue) --y;

            if (x <= y) swap(&array[x], &array[y]);

            ++x;
            --y;
        }
        quickSort(array, first, y);
        quickSort(array, x, last);
    }
}

int main() {
    int len;
    if (scanf("%d", &len) == EOF)
        return 0;
    int* array;
    array = (int*)malloc(len*sizeof(int));
    if (array == NULL)
        return 0;
    for (int i = 0; i < len; i++){
        if (scanf("%d", &array[i]) == EOF)
            return 0;
    }

    quickSort(array, 0, len-1);

    for (int i = 0; i < len; i++)
        printf("%d ", array[i]);
    free(array);
    return 0;
}
