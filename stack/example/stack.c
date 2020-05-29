#include "../stack_api.h"


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

    Stack *s_3 = Stack_push_move(10, Stack_free(&s_2));

    s_2 = Stack_reverse(s_3);

    Stack *s_4 = base_push_move(s_2, s_3, typeclass_stack);
    s_2 = Stack_clone(s_4);

    {
        int a;
        Stack_v(a, s_3);
        LINE_printf("%d\n", a);
    }

    {
        const char *a;
        Stack_v(a, Stack_value(s_4));
        LINE_printf("%s\n", a);
    }

    {
        const char *a;
        Stack_v(a, Stack_value(s_2));
        LINE_printf("%s\n", a);
    }
    Stack_clear(&s_4);
    Stack_clear(&s_2);
}
