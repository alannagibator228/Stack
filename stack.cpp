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

    hash_solver(stk);

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

    hash_solver(stk);

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

    hash_solver(stk);

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

    hash_solver(stk);
}

void mem_stack_smaller(Stack *stk)
{
    stack_aborter_validator(stk);
    stk->capacity = stk->capacity / MUL_DIV_COEF;
    stk->p_data_canary_left = (canary_t *)realloc(stk->p_data_canary_left, stk->capacity * sizeof(elem_t) + 2 * sizeof(canary_t));
    stk->p_data_canary_right = (canary_t *)((char *)stk->p_data_canary_left + sizeof(canary_t) + stk->capacity * sizeof(elem_t));
    stk->data = (elem_t *)((char *)stk->p_data_canary_left + sizeof(canary_t));

    *(stk->p_data_canary_right) = NORM;

    hash_solver(stk);
}

short stack_ok(Stack *stk)
{
    short error = NONE;
    if (stk == NULL)
    {
        error |= INVALID_STK;
        return error;
    }
    if (stk->stk_canary_left != stk->stk_canary_right)
    {
        error |= UNEQUAL_STK_CANARY;
    }
    if (stk->stk_canary_right != NORM)
    {
        error |= INVALID_STK_CANARY;
    }
    if (stk->capacity <= 0)
    {
        error |= INVALID_CAPACITY;
    }
    if (stk->size < 0)
    {
        error |= INVALID_SIZE;
    }
    if (stk->data == NULL || stk->p_data_canary_left == NULL || stk->p_data_canary_right == NULL)
    {
        error |= INVALID_DATA;
    }
    if (*(stk->p_data_canary_left) != *(stk->p_data_canary_right))
    {
        error |= UNEQUAL_DATA_CANARY;
    }
    if (*(stk->p_data_canary_right) != NORM)
    {
        error |= INVALID_DATA_CANARY;
    }

    hash_t old_stk_hash = stk->stack_hash;
    hash_t old_data_hash = stk->data_hash;
    hash_solver(stk);
    if (old_stk_hash != stk->stack_hash)
    {
        error |= BAD_STACK_HASH; 
    }
    if (old_data_hash != stk->data_hash)
    {
        error |= BAD_DATA_HASH; 
    }

    return error;
}

short print_error(short error)
{
    const char *error_text[15] =
        {
            "1  ERROR: Invalid stack pointer",
            "2  ERROR: Stack canary don`t equal",
            "3  ERROR: Invalid stack canaries",
            "4  ERROR: Invalid capacity",
            "5  ERROR: Invalid size",
            "6  ERROR: Invalid data or data_canaries pointers",
            "7  ERROR: Data canary don`t equal",
            "8  ERROR: Invalid data canaries",
            "9  ERROR: Bad stack_hash",
            "10 ERROR: Bad data_hash"
        };

    for (int i = 0; i < 8 * sizeof(short); i++)
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

hash_t HashFAQ6(const char * str, int size)
{
    unsigned int hash = 0;  	
    for (; size > 0; str++) 	
    { 		
        hash += (unsigned char)(*str); 		
        hash += (hash << 10); 		
        hash ^= (hash >> 6); 	
        size --;
    } 	
    hash += (hash << 3); 	
    hash ^= (hash >> 11); 	
    hash += (hash << 15); 

    return hash;  
} 

hash_t create_stack_hash (Stack * stk)
{
    char* p = (char*) stk;
    return HashFAQ6(p, sizeof(stk));
}

hash_t create_data_hash (Stack * stk)
{
    char* p = (char*) stk->data - sizeof(canary_t);
    return HashFAQ6(p, stk->capacity * sizeof(elem_t) + 2 * sizeof(canary_t));
}

void hash_solver(Stack *stk)
{
    stk->stack_hash = INIT_HASH;
    stk->data_hash = INIT_HASH;
    stk->stack_hash = create_stack_hash(stk);
    stk->data_hash = create_data_hash(stk);
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

    printf("\tstach_hash = %d\n", stk->stack_hash);
    printf("\tdata_hash = %d\n", stk->data_hash);

    printf("\n\n");

    return 0;
}