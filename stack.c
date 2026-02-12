#include <stdint.h>
#include "stack.h"

void stack_push(Stack* stk, int16_t val) {
    Stack* copy = stk;
    while (copy->next != NULL) {
        copy = copy->next;
    }

    copy->next = malloc(sizeof(Stack));
    copy->next->val = val;
    copy->next->next = NULL;

    return;
}

int stack_pop(Stack* stk) {
    Stack* copy = stk;
    while (copy->next != NULL) {
        copy = copy->next;
    }

    int val = copy->next->val;
    free(copy->next);
    copy->next = NULL;

    return val;
}
