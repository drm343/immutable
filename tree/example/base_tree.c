#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#include "../tree.h"


int main (void) {
    Tree *a = B(3, B(6, V(1), V(5)), V(2));
    TREE_DEBUG(a);

    const Tree *b = tree_right_static(a);
    TREE_DEBUG(b);

    Tree *c = B(4, NULL, tree_right_shared(a));
    TREE_DEBUG(a);

    Tree *d = B(0, tree_root_shared(c), NULL);

    TREE_DEBUG(c);
    TREE_DEBUG(d);

    tree_retain(&a);
    tree_retain(&c);
    tree_retain(&d);
    tree_sweep();
    printf("is null %d\n", NULL == d);
}
