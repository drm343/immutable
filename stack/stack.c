#include "stack.h"


#define TYPE_CLONEABLE(s) Cloneable typeclass = {s->clear, s->clone}


typedef struct _Stack {
    void * content;
    f_clear clear;
    f_clone clone;
    struct _Stack * prev;
} Stack;


bool is_cloneable(Cloneable typeclass) {
    return typeclass.clone != NULL;
}


Stack * base_create(void * stack, Cloneable typeclass) {
    return base_push(stack, NULL, typeclass);
}


Stack * base_create_move(const void * stack, Cloneable typeclass) {
    return base_push_move(stack, NULL, typeclass);
}


Stack * base_push(void * stack, Stack *prev, Cloneable typeclass) {
    if (stack == NULL) {
        return prev;
    }
    else if (is_cloneable(typeclass)) {
        return base_push_move(typeclass.clone(stack), prev, typeclass);
    }
    else {
        return prev;
    }
}


Stack * base_push_move(const void * stack, Stack *prev,
        Cloneable typeclass) {
    if (stack == NULL) {
        return prev;
    }
    else if (is_cloneable(typeclass)) {
        Stack *s = NULL;
        s = NEW(s);

        s->content = (void *)stack;
        s->clear = typeclass.clear;
        s->clone = typeclass.clone;
        s->prev = prev;
        return s;
    }
    else {
        return prev;
    }
}


Stack * Stack_free(Ref_Stack s_ptr) {
    Stack *s = *s_ptr;

    if (s != NULL) {
        Stack *prev = s->prev;

        if (s->clear != NULL) {
            s->clear(&(s->content));
        }

        free(s);
        *s_ptr = NULL;
        return prev;
    }
}


void Stack_clear(Ref_Stack s_ptr) {
    Stack *stack = *s_ptr;

    while (stack != NULL) {
        stack = Stack_free(&stack);
    }
    *s_ptr = NULL;
}


const Stack const * Stack_pop(const Stack const *s) {
    if (s != NULL) {
        return s->prev;
    }
    return NULL;
}


const void const * Stack_value(const Stack const *s) {
    if (s != NULL && s->content != NULL) {
        return s->content;
    }
    return NULL;
}


Stack * Stack_clone(const Stack const *origin) {
    const Stack const *s = origin;
    TYPE_CLONEABLE(s);

    if (s->prev == NULL) {
        return base_create(s->content, typeclass);
    }
    else {
        return base_push(s->content, Stack_clone(s->prev), typeclass);
    }
}


Stack * Stack_reverse(const Stack const *origin) {
    const Stack const *s = origin;
    Stack *new = NULL;

    while (s != NULL) {
        TYPE_CLONEABLE(s);

        if (new == NULL) {
            new = base_create(s->content, typeclass);
        }
        else {
            new = base_push(s->content, new, typeclass);
        }

        s = s->prev;
    }
    return new;
}
