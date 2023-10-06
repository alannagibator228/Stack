#include <stdio.h>
#include "stack.h"

int main()
{
    Stack stack = {};
    stack_Ctor(&stack, MINIMUM_CAPACITY);
    
    for (int i = 0; i < 20; i++)
    {
        stack_push(&stack, i);
    }
    
    stack_dump(&stack);

    elem_t x = 0;

    for (int i = 20; i > 0; i--)
    {
        stack_pop(&stack, &x);
    }

    stack_dump(&stack);

    stack_Dtor(&stack);

    return 0;
}