#ifndef STACK_H
#define STACK_H
#define MINIMUM_CAPACITY 8
#define MUL_DIV_COEF 2

typedef int elem_t;
typedef unsigned long size_t;

typedef struct
{
    elem_t *data;
    size_t size;
    size_t capacity;
} Stack;

int stack_Ctor(Stack *stk, size_t start_capacity);
int stack_Dtor(Stack *stk);

int stack_push(Stack *stk, elem_t value);
int stack_pop(Stack *stk, elem_t *value);

void mem_stack_bigger(Stack *stk);
void mem_stack_smaller(Stack *stk);

int stack_ok(Stack *stk);

#endif