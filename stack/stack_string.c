#include "stack_string.h"


static void string_clear(void **s) {
    char * str = *s;
    free(str);
    *s = NULL;
}


static void * string_clone(void *s) {
    return strdup(s);
}


Stack * Stack_String_create(void * stack) {
    Cloneable typeclass = {string_clear, string_clone};
    return base_create(stack, typeclass);
}


Stack * Stack_String_create_move(const void * stack) {
    Cloneable typeclass = {string_clear, string_clone};
    return base_create_move(stack, typeclass);
}


Stack * Stack_String_push(void * stack, Stack *prev) {
    Cloneable typeclass = {string_clear, string_clone};
    return base_push(stack, prev, typeclass);
}


Stack * Stack_String_push_move(const void * stack, Stack *prev) {
    Cloneable typeclass = {string_clear, string_clone};
    return base_push_move(stack, prev, typeclass);
}


const char *Stack_String_value(Stack *s) {
    return Stack_value(s);
}
