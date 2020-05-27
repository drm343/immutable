#ifndef STACK_STACK_DEFINE
#define STACK_STACK_DEFINE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define LINE_printf(f, ...) printf("%s:%d: " f, __FILE__, __LINE__, __VA_ARGS__);


#define NEW(OBJECT) malloc(sizeof(*OBJECT))


typedef void (*f_clear)(void **s);
typedef void *(*f_clone)(void *s);


typedef struct {
    f_clear clear;
    f_clone clone;
} Cloneable;


bool is_cloneable(Cloneable typeclass);


typedef struct _Stack Stack;
typedef Stack ** Ref_Stack;


Stack * base_create(void * stack, Cloneable typeclass);
Stack * base_create_move(const void * stack, Cloneable typeclass);
Stack * base_push(void * stack, Stack *prev, Cloneable typeclass);
Stack * base_push_move(const void * stack, Stack *prev, Cloneable typeclass);
Stack * Stack_create(void * stack);
Stack * Stack_create_move(const void * stack);
Stack * Stack_push(void * stack, Stack *prev);
Stack * Stack_push_move(const void * stack, Stack *prev);
Stack * Stack_free(Ref_Stack s_ptr);
void Stack_clear(Ref_Stack s_ptr);
const Stack const * Stack_pop(const Stack const *s);
const void const * Stack_value(const Stack const *s);
Stack * Stack_clone(const Stack const *origin);
Stack * Stack_reverse(const Stack const *s);
#endif
