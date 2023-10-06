#ifndef STACK_H
#define STACK_H
#define MINIMUM_CAPACITY 8
#define MUL_DIV_COEF 2
#define NORM 1297239886

typedef int elem_t;
typedef unsigned int canary_t;

typedef struct
{
    canary_t stk_canary_left;
    canary_t* p_data_canary_left;
    canary_t* p_data_canary_right;
    elem_t *data;
    int size;
    int capacity;
    canary_t stk_canary_right;
} Stack;

enum ERRRORS
{
    NONE = 0,
    VALID_STK = 1,
    EQUAL_STK_CANARY = 1 << 1,
    VALID_STK_CANARY = 1 << 2,
    VALID_CAPACITY = 1 << 3,
    VALID_SIZE = 1 << 4,
    VALID_DATA = 1 << 5,
    EQUAL_DATA_CANARY = 1 << 6,
    VALID_DATA_CANARY = 1 << 7
};

int stack_Ctor(Stack *stk, size_t start_capacity);
int stack_Dtor(Stack *stk);

int stack_push(Stack *stk, elem_t value);
int stack_pop(Stack *stk, elem_t *value);

void mem_stack_bigger(Stack *stk);
void mem_stack_smaller(Stack *stk);

short stack_ok(Stack *stk);
short print_error(short error);
void stack_aborter_validator(Stack * stk);

int print_canary(canary_t *value);
int stack_dump(Stack *stk);

#endif