#include <stdio.h>
#include <stdlib.h>

#include "../tree.h"


Tree *cons(Tree *car, Tree *cdr);
Tree *cons_int(int value, Tree *cdr);
Tree *car(Tree *root);
Tree *cdr(Tree *root);


Tree *cons(Tree *car, Tree *cdr) {
    return B(0, car, cdr);
}


Tree *cons_int(int value, Tree *cdr) {
    return B(0, tree_leaf(value), cdr);
}


Tree *cons_id(Tree *id) {
    return id;
}


#define CONS_B(b) _Generic((b),\
                    int: tree_leaf,\
                    default: cons_id\
                )(b)

#define CONS(a, b) _Generic((a),\
                    int: cons_int,\
                 default: cons\
                )(a, CONS_B(b))


void s_exp_debug(const Tree const *t) {
    if (t == NULL) {
        printf("nil");
        return;
    }
    else if (tree_is_leaf_static(t)) {
        printf("%d", tree_value_static(t));
    }
    else if (tree_is_node_static(t)) {
        printf("(");
        s_exp_debug(tree_left_static(t));
        printf(" ");
        s_exp_debug(tree_right_static(t));
        printf(")");
    }
}


#define S_EXP_DEBUG(a) s_exp_debug(a); printf("\n")


Tree *car(Tree *root) {
    if (root != NULL) {
        Tree *t = tree_left_shared(root);
        tree_mark(t);
        return t;
    }
    else {
        return NULL;
    }
}


Tree *cdr(Tree *root) {
    if (root != NULL) {
        Tree *t = tree_right_shared(root);
        tree_mark(t);
        return t;
    }
    else {
        return NULL;
    }
}


int main(void) {
    Tree *a = CONS(CONS(3, 0), CONS(2, NULL));
    tree_mark(a);
    S_EXP_DEBUG(a);
    S_EXP_DEBUG(car(a));
    S_EXP_DEBUG(cdr(a));
    S_EXP_DEBUG(car(cdr(a)));

    tree_sweep();
}
