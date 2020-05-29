#include "stack.c"
#include "stack_int.h"


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


static Cloneable typeclass = {int_clear, int_clone};


static Int *Int_new(int value) {
    Int *a = NULL;
    a = NEW(a);
    a->num = 10;
    return a;
}


int Stack_Int_value(Stack *s) {
    const Int *a = Stack_value(s);

    if (a != NULL) {
        return a->num;
    }
    else {
        return 0;
    }
}


Stack * Stack_Int_create(int value) {
    Int *v = Int_new(value);
    return base_create(v, typeclass);
}


Stack * Stack_Int_create_move(int value) {
    Int *v = Int_new(value);
    return base_create_move(v, typeclass);
}


Stack * Stack_Int_push(int value, Stack *prev) {
    Int *v = Int_new(value);
    return base_push(v, prev, typeclass);
}


Stack * Stack_Int_push_move(int value, Stack *prev) {
    Int *v = Int_new(value);
    return base_push_move(v, prev, typeclass);
}
