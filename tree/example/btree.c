#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../tree.h"


bool BTree_search(int value, Tree *root) {
    return tree_search(value, root);
}


Tree *BTree_insert(int value, Tree *root) {
    return tree_insert(value, root, base_gc_insert_colletor);
}


int main (void) {
    Tree *a = BTree_insert(3, NULL);

    a = BTree_insert(6, a);
    a = BTree_insert(1, a);
    a = BTree_insert(5, a);
    a = BTree_insert(2, a);
    a = BTree_insert(2, a);

    printf("is %d\n", BTree_search(2, a));

    Tree *b = B(4, NULL, tree_right_shared(a));

    TREE_DEBUG(a);
    tree_retain(&a);

    Tree *c = B(0, tree_root_shared(b), NULL);
    TREE_DEBUG(b);
    tree_retain(&b);

    TREE_DEBUG(c);
    tree_retain(&c);
    tree_sweep();
    printf("is null %d\n", NULL == a);
}
