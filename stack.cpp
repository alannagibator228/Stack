#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"

// TODO make a dump function (Prints all information)

int stack_Ctor(Stack *stk, size_t start_capacity)
{
    stk->capacity = start_capacity;
    (stk->data) = (elem_t *)calloc(stk->capacity, sizeof(elem_t));

    stk->size = 0;

    stack_ok(stk);
    return 0;
}

int stack_Dtor(Stack *stk)
{
    stk->size = 0;
    stk->capacity = 0;

    free(stk->data);

    return 0;
}

int stack_push(Stack *stk, elem_t value)
{
    stack_ok(stk);

    if (stk->size == stk->capacity)
    {
        mem_stack_bigger(stk);
    }

    stk->data[stk->size] = value;
    stk->size++;

    return 0;
}

int stack_pop(Stack *stk, elem_t *value)
{
    stack_ok(stk);
    assert(value != NULL);

    if (stk->size == 0)
    {
        return 0;
    }
    *value = stk->data[stk->size - 1];
    if ((stk->capacity >= 2 * MUL_DIV_COEF * stk->size) && (stk->capacity > MINIMUM_CAPACITY))
    {
        mem_stack_smaller(stk);
    }
    stk->size--;
    return 0;
}

void mem_stack_bigger(Stack *stk)
{
    stack_ok(stk);

    stk->capacity = stk->capacity * MUL_DIV_COEF;
    stk->data = (elem_t *)realloc(stk->data, stk->capacity * sizeof(elem_t));
}

void mem_stack_smaller(Stack *stk)
{
    stack_ok(stk);
    stk->capacity = stk->capacity / MUL_DIV_COEF;
    stk->data = (elem_t *)realloc(stk->data, stk->capacity * sizeof(elem_t));
}

int stack_ok(Stack *stk)
{
    assert(stk != NULL);
    assert(stk->capacity >= 0);
    assert(stk->size >= 0);
    assert(stk->data != NULL);

    return 0;
}