#ifndef STACK_STRING_DEFINE
#define STACK_STRING_DEFINE 1

#include "stack.h"

Stack * Stack_String_create(void * stack);
Stack * Stack_String_create_move(const void * stack);
Stack * Stack_String_push(void * stack, Stack *prev);
Stack * Stack_String_push_move(const void * stack, Stack *prev);
const char *Stack_String_value(Stack *s);

#endif
