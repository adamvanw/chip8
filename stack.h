#include <stdlib.h>

typedef struct Stack {
    int16_t val;
    struct Stack* next;
} Stack;

void stack_push(Stack*, int16_t);
int stack_pop(Stack*);
