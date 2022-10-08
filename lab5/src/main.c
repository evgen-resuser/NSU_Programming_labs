#include "buffer_read.h"
#include "buffer_write.h"
#include "list.h"
#include "tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHAR_SIZE 256
#define CHAR_BITS 8

typedef struct Code{
    char symbol;
    char code[CHAR_SIZE];
} Code;

Code createCode(const char symbol, const int len, const char* code){
    Code aNew = {symbol, ""};
    memcpy(aNew.code, code, len);
    return aNew;
}

//-------------------------

void fCloseAll(FILE* in, FILE* out){
    fclose(in);
    fclose(out);
}

//--------- huffTree funcs ---------

TreePtr convertListToHuffTree(ListPtr* const list){
    while (!isEmptyList(*list)){
        if (isEmptyList(listNext(*list))) break;

        int frequencySum = listFreq(*list);
        TreePtr left = popList(list);
        frequencySum += listFreq(*list);
        TreePtr right = popList(list);

        TreePtr result = createTree(' ', left, right);
        pushPriorityList(result, frequencySum, list);
    }
    return listTree(*list);
}

void outputHuffTreeVal(const TreePtr tree, WBuffer* buf){
    if (isEmpty(tree)) return;

    outputHuffTreeVal(leftTree(tree), buf);
    outputHuffTreeVal(rightTree(tree), buf);

    char bit = (isLeaf(tree)) ? '1' : '0';
    addBit(buf, bit);

    if (isEmpty(leftTree(tree)) && isEmpty(rightTree(tree))){
        unsigned char symbol = symbolFromTree(tree);
        for (int i = 0; i < CHAR_BITS; ++i){
            addBit(buf, (symbol & CHAR_SIZE / 2) == 0 ? '0' : '1');
            symbol <<= 1;
        }
    }
}

//-----------

ListPtr createLeavesList(FILE* in){
    int frequencyTable[CHAR_SIZE] = {0};
    RBuffer * buff = initBufR(in);
    if (buff == NULL) return NULL;

    unsigned char read;
    while (getChar(buff, &read) == 1)
        ++frequencyTable[read];

    ListPtr list = createList();

    for (int i = 0; i < CHAR_SIZE; ++i){
        if (frequencyTable[i] != 0){
            TreePtr tree = createTree((char)i, NULL, NULL);
            pushPriorityList(tree, frequencyTable[i], &list);
        }
    }
    freeRBuffer(buff);
    return list;
}

void fillHuffTable(const int lenOfCode, char* const symCode, Code* const huffTable, const TreePtr tree){
    if (isLeaf(tree)){
        static int indx = 0;
        huffTable[indx] = createCode(symbolFromTree(tree), lenOfCode, symCode);
        ++indx;
        return;
    }

    symCode[lenOfCode] = '0';
    fillHuffTable(lenOfCode+1, symCode, huffTable, leftTree(tree));
    symCode[lenOfCode] = '1';
    fillHuffTable(lenOfCode+1, symCode, huffTable, rightTree(tree));
    symCode[lenOfCode] = '\0';
}



//--------------------

void addSymbolCodeToBL_withHuffTable(const char symbol, Code* const huffTable, WBuffer* buf)
{
    int index = 0;
    while (huffTable[index].symbol != symbol) ++index;

    for (int i = 0; huffTable[index].code[i] != '\0'; ++i){
        addBit(buf, huffTable[index].code[i]);
    }
}

void extractSymbolFromBLUsingHuffmanTree(const TreePtr tree,
                                         WBuffer * buffW, RBuffer * buffR, WBuffer* buf){
    if (isLeaf(tree)){
        char writtenSymbol = symbolFromTree(tree);
        putChar(buffW, writtenSymbol);
        return;
    }

    unsigned char readSymbol;
    if (*bWindx(buf) == 0 && getChar(buffR, &readSymbol) == 1){
        addSymbolToBuff(buf, readSymbol);
    }

    char bit = readFirstBit(buf, 0);
    moveLine(buf);

    extractSymbolFromBLUsingHuffmanTree((bit == '0')? leftTree(tree) : rightTree(tree),
                                        buffW, buffR, buf);
}

void encoder(FILE* in, FILE* out){
    ListPtr list = createLeavesList(in);
    if (list == NULL)
        return;

    TreePtr huffTree = convertListToHuffTree(&list);

    int symCount = listFreq(list);
    if (fwrite(&symCount, sizeof(symCount), 1, out) != 1) return;

    int size = countLeaves(huffTree);
    Code* huffTable = calloc(size, sizeof(*huffTable));
    if (huffTable == NULL){
        destroyList(&list);
        treeCutter(&huffTree);
        return;
    }

    if (size == 1)
        huffTable[0] = createCode(symbolFromTree(huffTree), 1, "1");

    char symCode[CHAR_SIZE] = {0};
    fillHuffTable(0, symCode, huffTable, huffTree);

    if (fseek(in, sizeof(char), 0) != 0){
        destroyList(&list);
        treeCutter(&huffTree);
        free(huffTable);
        return;
    }

    WBuffer * buf = initBufW(out);

    outputHuffTreeVal(huffTree, buf);
    addBit(buf, '0');

    flushByIndx(buf);

    RBuffer * buffR = initBufR(in);
    if (buffR == NULL){
        destroyList(&list);
        treeCutter(&huffTree);
        free(huffTable);
        return;
    }

    char readSymbol;
    while (getChar(buffR, (unsigned char*)&readSymbol) == 1){
        addSymbolCodeToBL_withHuffTable(readSymbol, huffTable, buf);
        flushByIndx(buf);
    }

    if (*bWindx(buf) != 0)
    {
        for (size_t i = *bWindx(buf); i < CHAR_BITS; ++i) {
            addBit(buf, '0');
        }
        flushByIndx(buf);
    }

    freeWBuffer(buf);
    freeRBuffer(buffR);
    free(huffTable);
    destroyList(&list);
}



void decoder(FILE* in, FILE* out){
    int symCount = 0;
    if (fread(&symCount, sizeof(symCount), 1, in) != 1)
        return;

    TreePtr tree = NULL;
    ListPtr stack = createList();
    RBuffer * buffR = initBufR(in);
    if (buffR == NULL){
        return;
    }
    WBuffer* buf = initBufW(out);
    if (buf == NULL){
        freeRBuffer(buffR);
        return;
    }
    unsigned char first[1] = "";

    while(1){
        while ((first[0] != '0' || bWindx(buf) == 0) && *bWindx(buf) < CHAR_BITS+1){
            unsigned char readSym = '\0';
            if (getChar(buffR, &readSym) != 1) {
                freeWBuffer(buf);
                freeRBuffer(buffR);
                return;
            }
            addSymbolToBuff(buf, readSym);

            first[0] = readFirstBit(buf, 0);
        }

        if (first[0] == '1'){
            moveLine(buf);

            TreePtr leaf = createTree((char)bWbuff(buf)[0], NULL, NULL);

            for (int i = 0; i < 8; ++i) moveLine(buf);
            first[0] = readFirstBit(buf, 0);

            pushList(leaf, 0, &stack);
        } else {
            moveLine(buf);
            first[0] = readFirstBit(buf, 0);
            if (isEmptyList(listNext(stack))) {
                tree = popList(&stack);
                break;
            }

            int freqSum = listFreq(stack);
            TreePtr right = popList(&stack);
            freqSum += listFreq(stack);
            TreePtr left = popList(&stack);

            TreePtr result = createTree('\0', left, right);
            pushList(result, freqSum, &stack);
        }
    }




    WBuffer * buffW = initBufW(out);
    if (buffW == NULL){
        freeRBuffer(buffR);
        treeCutter(&tree);
        freeWBuffer(buf);
        return;
    }
    for (int i = 0; i < symCount; ++i)
        extractSymbolFromBLUsingHuffmanTree(tree, buffW, buffR, buf);
    flush(buffW);
    freeWBuffer(buf);
    freeWBuffer(buffW);
    freeRBuffer(buffR);
    treeCutter(&tree);
}

//--------------------

int main() {
    FILE* input = fopen("in.txt", "rb");
    if (input == NULL) return 0;
    FILE* output = fopen("out.txt", "wb");
    if (output == NULL){
        fclose(input);
        return 0;
    }

    char mode = '\0';
    if (fread(&mode, sizeof(mode), 1, input) != 1){
        fCloseAll(input, output);
        return 0;
    }

    switch(mode){
        case 'c':
            encoder(input, output);
            break;
        case 'd':
            decoder(input, output);
            break;
        default:
            break;
    }

    fCloseAll(input, output);

    return 0;
}
