#ifndef STACK_INT_DEFINE
#define STACK_INT_DEFINE 1

#include "stack.h"

typedef struct _int Int;


Stack * Stack_Int_create(int value);
Stack * Stack_Int_create_move(int value);
Stack * Stack_Int_push(int value, Stack *prev);
Stack * Stack_Int_push_move(int value, Stack *prev);
int Stack_Int_value(Stack *s);

#endif
