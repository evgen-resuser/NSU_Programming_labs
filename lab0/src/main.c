#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <stdio.h>
#include <string.h>

enum DotFinderConsts{
    NO_DOTS = -1,
    TWO_OR_MORE_DOTS = -2,
    UNREACHABLE_VALUE = -10,
};

enum StrSizes {
    STANDARD_SIZE = 20,
    RESULT_SIZE = 50,
};

int CheckBase(const int base) {
    if (base < 2 || base > 16) return 0;
    return 1;
}

// Searches for a dot that divides a number into integer and fractional parts
int DotFinder(const char number[STANDARD_SIZE], const int len) {
    int count = 0;
    int result = 0;
    for (int i = 0; i <= len; i++) {
        if (number[i] == '.' && i != len) {
            result = i;
            count++;
        }
    }

    switch (count) {
        case 0:
            return NO_DOTS; // no dots
        case 1:
            return result; // return the index of dot
        case 2:
            return TWO_OR_MORE_DOTS; // error (two or more dots)
        default: break;
    }
    return UNREACHABLE_VALUE;
}

// Conversion Char to Integer (using ASCII codes)
int CharToInt(const char symbol) {
    const char digits1[] = { "0123456789abcdef" };
    const char digits2[] = { "0123456789ABCDEF" };
    const int lenOfDigits = 15;
    for (int i = 0; i <= lenOfDigits; i++) {
        if ((symbol == digits1[i]) || (symbol == digits2[i])) return i;
    }
    return -1;
}

// Conversion from any number system to decimal.
double FromAnyToDec(const char number[STANDARD_SIZE], const int base1, const int len) {
    double result = 0.0;
    int dot = DotFinder(number, len);
    int sizeOfIntgrPart = (int)len;
    int sizeOfFrctPart = 0;

    if (dot != NO_DOTS) {
        sizeOfIntgrPart = dot;
        sizeOfFrctPart = (int)len - dot - 1;
    }

    for (int i = 0; i < sizeOfIntgrPart; i++) {
        result += CharToInt(number[i]) * pow(base1, sizeOfIntgrPart - i - 1);

    }

    if (sizeOfFrctPart != 0) {
        int k = 0;
        for (int i = dot + 1; i < len; i++) {
            result += CharToInt(number[i]) * pow(base1, -k - 1);
            k++;
        }
    }
    return result;
}

// Conversion from decimal number system to any
void FromDecToAny(long long int integerDecPart, double fractionalDecPart, const int base2, char* result) {
    const char digits[] = { "0123456789ABCDEF" };
    int k = 0;
    if (integerDecPart != 0) {
        char reversedIntPart[RESULT_SIZE];
        while (integerDecPart != 0) {
            reversedIntPart[k] = digits[integerDecPart % base2];
            k++;
            integerDecPart /= base2;
        }

        //
        int count = k - 1;
        for (int i = 0; i != k; i++) {
            result[i] = reversedIntPart[count];
            count--;
        }
    }
    else { k++; }
    if (fractionalDecPart == 0) {
        result[k] = '\0';
    }
    else {
        result[k] = '.';
        k++;
        const int maxLenOfFrctPart = 12;
        for (int i = 0; i <= maxLenOfFrctPart; i++) {
            int buffer;
            fractionalDecPart = fractionalDecPart * base2;
            buffer = (int)fractionalDecPart;
            result[k] = digits[buffer];
            fractionalDecPart = fractionalDecPart - (double)buffer;
            k++;
        }
        result[k] = '\0';
    }
}

int main() {
    int base1, base2; // Reading the Bases
    int scan = scanf("%d %d", &base1, &base2);
    if (scan == EOF || scan == 0 || scan == 1 || CheckBase(base1) == 0 || CheckBase(base2) == 0) {
        printf("bad input");
        return 0;
    }

    char number[STANDARD_SIZE];
    if (scanf("%19s", number) == EOF) {
        printf("bad input");
        return 0;
    } // Reading the number that should be converted

    const int len = (int)strlen(number);
    const int dot = DotFinder(number, len); // Searching for dot index(if there is one)
    int maxLen = 13; //Check: if there is a point, then the maximum length of the number is 13, otherwise 12 (by condition)
    if (dot == NO_DOTS) {
        maxLen--;
    }
    if (((dot == 0 || dot == len - 1) && len >= 1) || dot == TWO_OR_MORE_DOTS) {
        printf("bad input");
        return 0;
    }

    // Checking the number for characters that are not allowed for the number system
    for (int i = 0; i < len; i++) {
        if (i == dot) continue;
        int n = CharToInt(number[i]);
        if (n >= base1 || n == NO_DOTS) {
            printf("bad input");
            return 0;
        }
    } // If the length is greater than the maximum (with/without point), then "bad input"
    if (len > maxLen) {
        printf("bad input");
        return 0;
    }

    double decNumber = FromAnyToDec(number, base1, len);
    long long int intgrPart = (long long int)decNumber; // Integer part
    double frctPart = decNumber - (long long int)decNumber; // Fractional part

    char result[RESULT_SIZE] = { '0' };
    FromDecToAny(intgrPart, frctPart, base2, result);
    printf("%s", result);
    return 0;
}
