#include "../stack.h"


typedef struct _int {
    int num;
} Int;


static void int_clear(void **s) {
    Int *a = *s;
    free(a);
    *s = NULL;
}


static void * int_clone(void *s) {
    Int *old = s;
    Int *a;
    a = NEW(a);
    a->num = old->num;
    return a;
}


Cloneable typeclass_int = {int_clear, int_clone};


static void stack_clear(void **s) {
    Stack *p = *s;
    Stack_clear(&p);
    *s = NULL;
}
static void * stack_clone(void *s) {
    Stack *result = Stack_clone(s);
    return result;
}
Cloneable typeclass_stack = {stack_clear, stack_clone};


int main (void) {
    Stack *s_1 = Stack_create("哈囉");
    if (s_1 == NULL) {
        LINE_printf("can't create\n", NULL);
    }
    LINE_printf("%s\n", Stack_value(s_1));
    Stack *s_2 = Stack_push("world", s_1);

    Int *a;
    a = NEW(a);
    a->num = 10;
    Stack *s_3 = base_push_move(a, Stack_free(&s_2), typeclass_int);

    s_2 = Stack_reverse(s_3);

    Stack *s_4 = base_push_move(s_2, s_3, typeclass_stack);
    s_2 = Stack_clone(s_4);

    LINE_printf("%d\n", ((Int *)Stack_value(s_3))->num);
    LINE_printf("%s\n", Stack_value(Stack_value(s_4)));
    LINE_printf("%s\n", Stack_value(Stack_value(s_2)));
    Stack_clear(&s_4);
    Stack_clear(&s_2);
}
