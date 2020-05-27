#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#include "../tree.c"
#include "../../stack/stack.h"


typedef struct _LR {
    bool is_left;
    Tree *root;
} LR;


static void LR_clear(void **s) {
    LR *a = *s;
    a->root = NULL;
    free(a);
    *s = NULL;
}


static void * LR_clone(void *s) {
    LR *old = s;
    LR *a;
    a = NEW(a);
    a->is_left = old->is_left;
    a->root = old->root;
    return a;
}
Cloneable typeclass_LR = {LR_clear, LR_clone};


Stack *insert_rollback(Tree *root, Stack *s, bool is_left) {
    LR *a;
    a = NEW(a);
    a->is_left = is_left;
    a->root = root;

    if (s == NULL) {
        return base_create_move(a, typeclass_LR);
    }
    else {
        return base_push_move(a, s, typeclass_LR);
    }
}


Stack *insert_left(Tree *root, Stack *s) {
    return insert_rollback(root, s, true);
}


Stack *insert_right(Tree *root, Stack *s) {
    return insert_rollback(root, s, false);
}


Tree *avl_insert_colletor(int value, Tree *root, Stack **s) {
    if (root == NULL) {
        return tree_leaf(value);
    }
    else if (tree_is_leaf(root)) {
        if (value > root->value) {
            Tree *t = B(root->value, NULL, V(value));
            *s = insert_right(t, *s);
            return t;
        }
        else {
            Tree *t = B(root->value, V(value), NULL);
            *s = insert_left(t, *s);
            return t;
        }
    }
    else if (tree_is_node(root)) {
        int t_value = tree_value(root);

        if (value > t_value) {
            Tree *t = B(t_value,
                    tree_left_shared(root),
                    avl_insert_colletor(value, root->right, s));
            *s = insert_right(t, *s);
            return t;
        }
        else {
            Tree *t = B(t_value,
                    avl_insert_colletor(value, root->left, s),
                    tree_right_shared(root));
            *s = insert_left(t, *s);
            return t;
        }
    }
    else {
        return NULL;
    }
}


Tree *rotate_right(const Tree const *root) {
    //     0         1
    //    / \       / \
    //   1   n ->  2   0
    //  / \
    // 2   n
    const Tree *b = tree_left_static(root);
    Tree *tmp = B(root->value, tree_right_copy(b), tree_right_copy(root));
    Tree *t = B(b->value, tree_left_copy(b), tmp);
    return t;
}


Tree *rotate_left(const Tree const *root) {
    //     0           1
    //    / \         / \
    //   n   1   ->  0   2
    //      / \
    //     n   2
    const Tree *b = tree_right_static(root);
    Tree *tmp = B(root->value, tree_left_copy(root), tree_left_copy(b));
    Tree *t = B(b->value, tmp, tree_right_copy(b));
    return t;
}


Tree *rotate_left_right(Tree *root) {
    //     0         2
    //    / \       / \
    //   1   n ->  1   0
    //  / \
    // n   2
    const Tree *b = tree_left_static(root);
    const Tree *c = tree_right(root->left);

    Tree *t_1 = rotate_left(b);

    Tree *tmp = B(root->value, tree_right_shared(t_1), tree_right_shared(root));
    Tree *t = B(t_1->value, tree_left_shared(t_1), tmp);
    tree_retain(&t_1);
    return t;
}


Tree *rotate_right_left(Tree *root) {
    //     0           2
    //    / \         / \
    //   n   1   ->  0   1
    //      / \
    //     2   n
    const Tree *b = tree_right_static(root);
    const Tree *c = tree_left_static(root->right);

    Tree *t_1 = rotate_right(b);

    Tree *tmp = B(root->value,
            tree_left_shared(root),
            tree_left_shared(t_1));
    Tree *t = B(t_1->value, tmp, tree_right_shared(t_1));
    tree_retain(&t_1);
    return t;
}


int max(int a, int b) {
    return (a >= b? a:b);
}


int avl_hieght(Tree *root) {
    if (root == NULL) {
        return -1;
    }
    else if (tree_is_leaf(root)) {
        return 0;
    }
    else if (tree_is_node(root)) {
        return max(avl_hieght(root->left), avl_hieght(root->right)) + 1;
    }
    else {
        return -1;
    }
}


Tree *avl_update(Tree *root, Stack **stack) {
    Stack *s = *stack;
    if (s == NULL) {
        return root;
    }
    else {
        const LR *lr_last = Stack_value(s);

        if (lr_last == NULL) {
            return lr_last->root;
        }
        else if (lr_last->is_left) {
            Tree *t = lr_last->root;
            Tree *result = B(t->value, root, tree_right_shared(t));
            s = Stack_free(stack);
            return avl_update(result, &s);
        }
        else {
            Tree *t = lr_last->root;
            Tree *result = B(t->value, tree_left_shared(t), root);
            s = Stack_free(stack);
            return avl_update(result, &s);
        }
    }
}


Tree *avl_insert(int value, Tree *root) {
    if (tree_search(value, root)) {
        return root;
    }
    tree_mark(root);
    Stack *s = NULL;
    Tree *t = avl_insert_colletor(value, root, &s);

    if (s != NULL) {
        Stack *current = Stack_reverse(s);
        Stack_clear(&s);

        while (current != NULL) {
            const LR *lr_last = Stack_value(current);
            const LR *lr_root = Stack_value(Stack_pop(current));

            int h_right = 0;
            int h_left = 0;

            if (lr_last == NULL) {
                s = current;
                break;
            }
            else if (lr_root == NULL) {
                s = current;
                break;
            }
            else if (lr_root->is_left) {
                h_left = avl_hieght(lr_last->root);
                h_right = avl_hieght(lr_root->root->right);
            }
            else {
                h_left = avl_hieght(lr_root->root->left);
                h_right = avl_hieght(lr_last->root);
            }

            int bf = (h_left - h_right);
            if (bf >= 2) {
                if (lr_last == NULL) {
                    s = current;
                    break;
                }
                else if (lr_last->is_left == true) {
                    tree_mark(t);
                    t = rotate_right(t);
                    current = Stack_free(&current);
                    current = Stack_free(&current);
                    t = avl_update(t, &current);
                    s = current;
                    break;
                }
                else {
                    tree_mark(t);
                    t = rotate_left_right(t);
                    current = Stack_free(&current);
                    current = Stack_free(&current);
                    t = avl_update(t, &current);
                    s = current;
                    break;
                }
            }
            else if (bf <= -2) {
                if (lr_last == NULL) {
                    s = current;
                    break;
                }
                else if (lr_last->is_left) {
                    tree_mark(t);
                    t = rotate_right_left(t);
                    current = Stack_free(&current);
                    current = Stack_free(&current);
                    t = avl_update(t, &current);
                    s = current;
                    break;
                }
                else {
                    tree_mark(t);
                    t = rotate_left(t);
                    current = Stack_free(&current);
                    current = Stack_free(&current);
                    t = avl_update(t, &current);
                    s = current;
                    break;
                }
            }

            current = Stack_free(&current);
        }
    }
    Stack_clear(&s);
    return t;
}


int main (void) {
    {
        Tree *a = avl_insert(2, NULL);
        a = avl_insert(1, a);
        a = avl_insert(0, a);
        tree_mark(a);
        LINE_printf("B(2, B(1, V(0), NULL), NULL)\n", NULL);
        TREE_DEBUG(a);

        tree_sweep();
    }

    {
        Tree *a = avl_insert(0, NULL);
        a = avl_insert(1, a);
        a = avl_insert(2, a);
        tree_mark(a);
        LINE_printf("B(0, NULL, B(1, NULL, V(2)))\n", NULL);
        TREE_DEBUG(a);

        tree_sweep();
    }

    {
        Tree *a = avl_insert(2, NULL);
        a = avl_insert(0, a);
        a = avl_insert(1, a);
        tree_mark(a);
        LINE_printf("B(2, B(0, NULL, V(1)), NULL)\n", NULL);
        TREE_DEBUG(a);

        tree_sweep();
    }

    {
        Tree *a = avl_insert(0, NULL);
        a = avl_insert(2, a);
        a = avl_insert(1, a);
        tree_mark(a);
        LINE_printf("B(0, NULL, B(2, V(1), NULL))\n", NULL);
        TREE_DEBUG(a);

        tree_sweep();
    }
}
