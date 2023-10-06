#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"

int stack_Ctor(Stack *stk, size_t start_capacity)
{
    stk->stk_canary_left = stk->stk_canary_right = NORM;

    stk->capacity = start_capacity;
    stk->p_data_canary_left = (canary_t *)calloc(1, stk->capacity * sizeof(elem_t) + 2 * sizeof(canary_t));
    stk->p_data_canary_right = (canary_t *)((char *)stk->p_data_canary_left + sizeof(canary_t) + stk->capacity * sizeof(elem_t));
    stk->data = (elem_t *)((char *)stk->p_data_canary_left + sizeof(canary_t));

    *(stk->p_data_canary_left) = NORM;
    *(stk->p_data_canary_right) = NORM;

    stk->size = 0;

    stack_aborter_validator(stk);

    return 0;
}

int stack_Dtor(Stack *stk)
{
    stk->size = -1;
    stk->capacity = -1;

    free(stk->p_data_canary_left);

    return 0;
}

int stack_push(Stack *stk, elem_t value)
{
    stack_aborter_validator(stk);

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
    stack_aborter_validator(stk);
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
    stack_aborter_validator(stk);

    stk->capacity = stk->capacity * MUL_DIV_COEF;
    stk->p_data_canary_left = (canary_t *)realloc(stk->p_data_canary_left, stk->capacity * sizeof(elem_t) + 2 * sizeof(canary_t));
    stk->p_data_canary_right = (canary_t *)((char *)stk->p_data_canary_left + sizeof(canary_t) + stk->capacity * sizeof(elem_t));
    stk->data = (elem_t *)((char *)stk->p_data_canary_left + sizeof(canary_t));

    *(stk->p_data_canary_right) = NORM;
}

void mem_stack_smaller(Stack *stk)
{
    stack_aborter_validator(stk);
    stk->capacity = stk->capacity / MUL_DIV_COEF;
    stk->p_data_canary_left = (canary_t *)realloc(stk->p_data_canary_left, stk->capacity * sizeof(elem_t) + 2 * sizeof(canary_t));
    stk->p_data_canary_right = (canary_t *)((char *)stk->p_data_canary_left + sizeof(canary_t) + stk->capacity * sizeof(elem_t));
    stk->data = (elem_t *)((char *)stk->p_data_canary_left + sizeof(canary_t));

    *(stk->p_data_canary_right) = NORM;
}

short stack_ok(Stack *stk)
{
    short error = NONE;
    if (stk == NULL)
    {
        error |= VALID_STK;
        return error;
    }
    if (stk->stk_canary_left != stk->stk_canary_right)
    {
        error |= EQUAL_STK_CANARY;
    }
    if (stk->stk_canary_right != NORM)
    {
        error |= VALID_STK_CANARY;
    }
    if (stk->capacity <= 0)
    {
        error |= VALID_CAPACITY;
    }
    if (stk->size < 0)
    {
        error |= VALID_SIZE;
    }
    if (stk->data == NULL || stk->p_data_canary_left == NULL || stk->p_data_canary_right == NULL)
    {
        error |= VALID_DATA;
    }
    if (*(stk->p_data_canary_left) != *(stk->p_data_canary_right))
    {
        error |= EQUAL_DATA_CANARY;
    }
    if (*(stk->p_data_canary_right) != NORM)
    {
        error |= VALID_DATA_CANARY;
    }

    return error;
}

short print_error(short error)
{
    const char *error_text[15] =
        {
            "1 ERROR: Invalid stack pointer",
            "2 ERROR: Stack canary don`t equal",
            "3 ERROR: Invalid stack canaries",
            "4 ERROR: Invalid capacity",
            "5 ERROR: Invalid size",
            "6 ERROR: Invalid data or data_canaries pointers",
            "7 ERROR: Data canary don`t equal",
            "8 ERROR: Invalid data canaries"};

    for (int i = 0; i < 8; i++)
    {
        if (((error | (1 << i))) == error)
        {
            printf("|%s|\n", error_text[i]);
        }
    }
    return error;
}

void stack_aborter_validator(Stack *stk)
{
    if (print_error(stack_ok(stk)) != NONE)
    {
        stack_dump(stk);
        abort();
    }
}

int print_canary(canary_t *value)
{
    if (value != NULL)
    {
        char *p = (char *)(value);
        for (int i = 0; i < 4; i++)
        {
            printf("%c", p[i]);
        }
        printf("\n");
    }
    else
    {
        printf("NaN\n");
    }
    return 0;
}

int stack_dump(Stack *stk)
{
    // print_error(stack_ok(stk));
    if (stk == NULL)
    {
        return 0;
    }
    printf("Stack [%p]\n", stk);

    printf("\tstk_canary_left = ");
    print_canary(&stk->stk_canary_left);

    printf("\tstk_canary_right = ");
    print_canary(&stk->stk_canary_right);

    printf("\n");

    printf("\tsize = %d\n", stk->size);
    printf("\tcapacity = %d\n", stk->capacity);

    printf("\n");

    printf("\tdata_canary_left [%p] = ", stk->p_data_canary_left);
    print_canary(stk->p_data_canary_left);

    printf("\tdata_canary_right [%p] = ", stk->p_data_canary_right);
    print_canary(stk->p_data_canary_right);

    printf("\n");

    printf("\tarray [%p] =\n\n", stk->data);
    if (stk->data == NULL)
    {
        return 0;
    }
    for (int num = 0; num < stk->capacity; num++)
    {
        if (num < stk->size)
        {
            printf("\t\tarr[%2d] = %d;\n", num, stk->data[num]);
        }
        else
        {
            printf("\t\tarr[%2d] = NaN;\n", num);
        }
    }
    printf("\n");

    return 0;
}