#include <stdio.h>
#include "stack.h"

int main()
{
    Stack stack = {};
    stack_Ctor(&stack, MINIMUM_CAPACITY);
    for (int i = 0; i < 20; i++)
    {
        stack_push(&stack, i);
        printf("%3d  %3ld/%3ld\n",stack.data[stack.size -1], stack.size, stack.capacity);
    }

    elem_t x = 0;

    for (int i = 20; i > 0; i--)
    {
        stack_pop(&stack, &x);
        if (stack.size >= 1)
        {
            printf("%3d  %3ld/%3ld\n",stack.data[stack.size - 1], stack.size, stack.capacity);
        }

    }
    stack_Dtor(&stack);

    return 0;
}