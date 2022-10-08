#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

#define LEFT_BRACKET 0
#define RIGHT_BRACKET 1

#define INPUT_SIZE 1001

enum codes {
    SUCCESS = 0,
    FAILURE = -1,
    SYNTAX_ERROR = -2,
    DIVISION_BY_ZERO = -3,
};
///////////
typedef struct stack {
    char values[2*INPUT_SIZE];
    size_t pos;
} stack;

void createStack(stack* stck) {
    stck -> pos = 0;
}

int isEmpty(stack stck) {
    return stck.pos == 0;
}

void stckRemove(stack* stck) {
    assert(!isEmpty(*stck));
    --stck -> pos;
}

char getTop(const stack* stck) {
    assert(stck -> pos != 0);
    return stck->values[stck->pos-1];
}

char pop(stack* stck) {
    assert(!isEmpty(*stck));
    char top = getTop(stck);
    stckRemove(stck);
    return top;
}

void push(stack* stck, char value) {
    stck -> values[stck -> pos] = value;
    ++stck -> pos;
}
///////////////

int isDigit(char a) {
    return ('0' <= a && a <= '9');
}

int isOperation(char a) {
    return (a == '+' || a == '-' || a == '*' || a == '/');
}

int inputCheck(char* input) {
    size_t brackets[2] = { 0, 0 };
    size_t leftBracketIndx = INPUT_SIZE;
    size_t operatorIndx = -1;

    size_t indx = 0;
    while (indx < INPUT_SIZE + 1) {
        input[indx] = (char)getchar();
        switch (input[indx]) {
            case '(':
                ++brackets[LEFT_BRACKET];
                leftBracketIndx = indx;
                break;
            case ')':
                if (operatorIndx + 1 >= indx || leftBracketIndx + 1 >= indx) return SYNTAX_ERROR;
                ++brackets[RIGHT_BRACKET];
                break;
            case EOF:
                return SYNTAX_ERROR;
            case '\n':
                if (indx == 0 || brackets[LEFT_BRACKET] != brackets[RIGHT_BRACKET] || operatorIndx + 1 >= indx) return SYNTAX_ERROR;
                input[indx] = '\0';
                return SUCCESS;
            default:
                if (isDigit(input[indx]) == TRUE) break;

                if (isOperation(input[indx]) == TRUE){
                    if (indx == operatorIndx + 1) return SYNTAX_ERROR;
                    else {
                        operatorIndx = indx;
                        break;
                    }
                }
                return SYNTAX_ERROR;
        }
        if (indx == INPUT_SIZE) return SYNTAX_ERROR;
        ++indx;
    }
    return SUCCESS;
}

int operationsPriority(char sign) {
    switch (sign) {
        case '(': return 0;

        case '+': goto pos1;
        case '-': pos1: return 1;

        case '*': goto pos2;
        case '/': pos2: return 2;

        default: return FAILURE;
    }
    return FAILURE;
}

void createPostfix(const char* input, char* postfixInput) {
    size_t indx = 0;
    stack operationsStack;
    createStack(&operationsStack);

    while (1) {
        if (input[indx] == '\0') break;
        if (isDigit(input[indx]) == TRUE) {
            do {
                *postfixInput = input[indx];
                ++postfixInput; //!
                indx++;
            } while (isDigit(input[indx]) == TRUE);
            *postfixInput = '~';
            ++postfixInput;
        } else {
            if (input[indx] == '(') push(&operationsStack, input[indx]);
            else if (input[indx] == ')') {
                while (getTop(&operationsStack) != '(') {
                    *postfixInput = pop(&operationsStack);
                    ++postfixInput;
                }
                stckRemove(&operationsStack);
            }
            else {
                while (!isEmpty(operationsStack) && operationsPriority(getTop(&operationsStack)) >=
                                                    operationsPriority(input[indx])) {
                    *postfixInput = pop(&operationsStack);
                    ++postfixInput;
                }
                push(&operationsStack, input[indx]);
            }
            ++indx;
        }
    }
    while (!isEmpty(operationsStack)) {
        *postfixInput = pop(&operationsStack);
        ++postfixInput;
    }
    *postfixInput = '\0';

    while (!isEmpty(operationsStack)) stckRemove(&operationsStack);
}


int charToDigit(char a) {
    return a - '0';
}

char digitToChar(int a) {
    return a + '0';
}

int simpleCalculation(char operation, int a, int b, int* error) {
    switch (operation) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) {
                *error = DIVISION_BY_ZERO;
                return FAILURE;
            }
            else return a / b;
        default: return FAILURE;
    }
    return FAILURE;
}

void turnStrIntoNum(int value, stack* numberStck) {
    if (value < 0) {
        value *= -1;
        push(numberStck, '-');
    }

    int pwr = 1;
    while (value > pwr * 10) pwr *= 10;

    while (pwr != 0) {
        push(numberStck, digitToChar(value / pwr));
        value %= pwr;
        pwr /= 10;
    }
    push(numberStck, '~'); //symbol-separator
}

int turnNumIntoStr(stack* numberStck) {
    stckRemove(numberStck);

    int pwr = 1;
    int value = 0;

    do {
        if (isEmpty(*numberStck) == FALSE && getTop(numberStck) == '-') {
            value *= -1;
            break;
        }
        value += charToDigit(pop(numberStck)) * pwr;
        pwr *= 10;
    } while (isEmpty(*numberStck) == FALSE && getTop(numberStck) != '~');
    return value;
}

int calculator(const char* input, int* error) {
    stack numberStack;
    createStack(&numberStack);
    char postfixExpression[INPUT_SIZE * 2];

    createPostfix(input, postfixExpression);

    size_t indx = 0;

    while (postfixExpression[indx] != '\0') {
        if (isDigit(postfixExpression[indx]) == TRUE) {
            do {
                push(&numberStack, postfixExpression[indx]);
                ++indx;
            } while (postfixExpression[indx] != '~');

            push(&numberStack, postfixExpression[indx]);
        } else {
            int a = turnNumIntoStr(&numberStack);
            int b = turnNumIntoStr(&numberStack);
            int c = simpleCalculation(postfixExpression[indx], b, a, error);

            if (*error != SUCCESS) return 0;

            turnStrIntoNum(c, &numberStack);
        }
        ++indx;
    }

    int answer = turnNumIntoStr(&numberStack);
    while (!isEmpty(numberStack)) stckRemove(&numberStack);

    return answer;
}

int main() {
    char input[INPUT_SIZE];
    if (inputCheck(input) == SYNTAX_ERROR) {
        printf("syntax error");
        return SUCCESS;
    }

    int error = SUCCESS;
    int answer = calculator(input, &error);

    switch (error) {
        case DIVISION_BY_ZERO:
            printf("division by zero");
            break;
        case SYNTAX_ERROR:
            printf("syntax error");
            break;
        case SUCCESS:
            printf("%d", answer);
            break;
        default: return FAILURE;
    }
    return SUCCESS;
}
