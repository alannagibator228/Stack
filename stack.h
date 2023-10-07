#ifndef STACK_H
#define STACK_H
#define MINIMUM_CAPACITY 8
#define MUL_DIV_COEF 2
#define NORM 1297239886
#define INIT_HASH 0

typedef int elem_t;
typedef unsigned int canary_t;
typedef unsigned int hash_t;

typedef struct
{
    canary_t stk_canary_left;
    int size;
    int capacity;
    canary_t *p_data_canary_left;
    canary_t *p_data_canary_right;
    elem_t *data;
    hash_t stack_hash;
    hash_t data_hash;
    canary_t stk_canary_right;
} Stack;

enum ERRRORS
{
    NONE = 0,
    INVALID_STK = 1,
    UNEQUAL_STK_CANARY = 1 << 1,
    INVALID_STK_CANARY = 1 << 2,
    INVALID_CAPACITY = 1 << 3,
    INVALID_SIZE = 1 << 4,
    INVALID_DATA = 1 << 5,
    UNEQUAL_DATA_CANARY = 1 << 6,
    INVALID_DATA_CANARY = 1 << 7,
    BAD_STACK_HASH = 1 << 8,
    BAD_DATA_HASH = 1 << 9
};

int stack_Ctor(Stack *stk, size_t start_capacity);
int stack_Dtor(Stack *stk);

int stack_push(Stack *stk, elem_t value);
int stack_pop(Stack *stk, elem_t *value);

void mem_stack_bigger(Stack *stk);
void mem_stack_smaller(Stack *stk);

short stack_ok(Stack *stk);
short print_error(short error);
void stack_aborter_validator(Stack *stk);

hash_t HashFAQ6(const char *str, int size);
hash_t create_stack_hash(Stack *stk);
hash_t create_data_hash(Stack *stk);
void hash_solver(Stack *stk);

int print_canary(canary_t *value);
int stack_dump(Stack *stk);

#endif