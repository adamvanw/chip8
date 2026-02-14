#include <stdint.h>
#include "stack.h"

void stack_push(Stack* stk, int16_t val) {
    Stack* copy = stk;
    if (stk == NULL) {
        stk = malloc(sizeof(Stack));
        stk->val = val;
        stk->next = NULL;
        return;
    }
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
    if (stk == NULL) {
        return -1;
    }
    while (copy->next != NULL) {
        copy = copy->next;
    }

    int val = copy->next->val;
    free(copy->next);
    copy->next = NULL;

    return val;
}
