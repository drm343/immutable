#include "tree.h"


enum _T_Type {
    Leaf,
    Node,
};


typedef struct _tree {
    enum _T_Type type;
    int value;
    int rc;
    struct _tree *left;
    struct _tree *right;
} Tree;


void tree_free(Tree **t_ptr);
void tree_retain(Tree **t_ptr);
void tree_release(Tree **t_ptr);


Tree *tree_node(enum _T_Type type, int value, Tree *left, Tree *right) {
    Tree *t;
    t = malloc(sizeof(*t));
    t->type = type;
    t->rc = 1;
    t->value = value;
    t->left = left;
    t->right = right;
    return t;
}


Tree *tree_leaf(int value) {
    return tree_node(Leaf, value, NULL, NULL);
}


Tree *tree_branch(int value, Tree *left, Tree *right) {
    return tree_node(Node, value, left, right);
}


const Tree const *tree_left(Tree *root) {
    return root->left;
}


const Tree const *tree_right(Tree *root) {
    return root->right;
}


const Tree const *tree_left_static(const Tree const *root) {
    return root->left;
}


const Tree const *tree_right_static(const Tree const *root) {
    return root->right;
}


Tree *tree_root_shared(Tree *root) {
    if (root != NULL) {
        root->rc++;
    }
    return root;
}


Tree *tree_left_shared(Tree *root) {
    if (root != NULL && root->left != NULL) {
        root->left->rc++;
    }
    return root->left;
}


Tree *tree_left_copy(const Tree const *root) {
    if (root != NULL && root->left != NULL) {
        root->left->rc++;
    }
    return root->left;
}


Tree *tree_right_shared(Tree *root) {
    if (root != NULL && root->right != NULL) {
        root->right->rc++;
    }
    return root->right;
}


Tree *tree_right_copy(const Tree const *root) {
    if (root != NULL && root->right != NULL) {
        root->right->rc++;
    }
    return root->right;
}


Tree *tree_left_move(Tree *root) {
    Tree *t = root->left;
    root->left = NULL;

    if (root->left == NULL && root->right == NULL) {
        root->type = Leaf;
    }
    return t;
}


Tree *tree_right_move(Tree *root) {
    Tree *t = root->right;
    root->right = NULL;

    if (root->left == NULL && root->right == NULL) {
        root->type = Leaf;
    }
    return t;
}


bool tree_is_leaf(Tree *root) {
    return tree_is_leaf_static(root);
}


bool tree_is_leaf_static(const Tree const *root) {
    return (root->type == Leaf);
}


bool tree_is_node(Tree *root) {
    return tree_is_node_static(root);
}


bool tree_is_node_static(const Tree const *root) {
    return (root->type == Node);
}


int tree_value(Tree *root) {
    return tree_value_static(root);
}


int tree_value_static(const Tree const *root) {
    return root->value;
}


void tree_free(Tree **t_ptr) {
    Tree *t = *t_ptr;

    free(t);
    *t_ptr = NULL;
}


void tree_retain(Tree **t_ptr) {
    Tree *t = *t_ptr;

    if (t == NULL) {
        return;
    }

    t->rc--;
    if (t->rc <= 0) {
        Tree *left = t->left;
        Tree *right = t->right;
        tree_retain(&left);
        tree_retain(&right);
        tree_free(t_ptr);
    }
}


void tree_release(Tree **t_ptr) {
    Tree *t = *t_ptr;

    if (t->left != NULL) {
        printf("free left\n");
        tree_free(&(t->left));
    }

    if (t->right != NULL) {
        printf("free right\n");
        tree_free(&(t->right));
    }

    free(t);
    *t_ptr = NULL;
}


void tree_debug(const Tree const *t) {
    if (t == NULL) {
        printf("nil");
        return;
    }

    switch (t->type) {
        case Leaf:
            printf("%d:%d", t->value, t->rc);
            break;
        case Node:
            printf("(");
            printf("%d:%d", t->value, t->rc);
            printf(" , ");
            tree_debug(t->left);
            printf(" , ");
            tree_debug(t->right);
            printf(")");
            break;
    }
}


#define V(a) tree_leaf(a)
#define B(n, a, b) tree_branch(n, a, b)
#define TREE_DEBUG(a) tree_debug(a); printf("\n");


bool tree_search(int value, Tree *root) {
    if (root == NULL) {
        return false;
    }
    else if (root->value == value) {
        return true;
    }
    else {
        bool r_left = false;
        bool r_right = false;

        if (root->left != NULL) {
            r_left = tree_search(value, root->left);
        }

        if (root->right != NULL) {
            r_right = tree_search(value, root->right);
        }

        return r_left || r_right;
    }
}


Tree *base_gc_insert_colletor(int value, Tree *root) {
    tree_mark(root);
    return base_insert_colletor(value, root);
}


Tree *base_insert_colletor(int value, Tree *root) {
    if (root == NULL) {
        return tree_leaf(value);
    }
    else if (root->type == Leaf) {
        if (value > root->value) {
            return B(root->value, NULL, V(value));
        }
        else {
            return B(root->value, V(value), NULL);
        }
    }
    else if (root->type == Node) {
        if (value > root->value) {
            return B(root->value,
                    tree_left_shared(root),
                    base_insert_colletor(value, root->right));
        }
        else {
            return B(root->value,
                    base_insert_colletor(value, root->left),
                    tree_right_shared(root));
        }
    }
    else {
        return NULL;
    }
}


typedef struct _TREE_GC {
    Tree *root;
    struct _TREE_GC *next;
} TREE_GC;


static TREE_GC *gc = NULL;


void tree_mark(Tree *root) {
    TREE_GC *next = gc;

    gc = malloc(sizeof(*gc));
    gc->root = root;
    gc->next = next;
}


void tree_sweep(void) {
    TREE_GC *current = gc;

    while (current != NULL) {
        TREE_GC *next = current->next;
        tree_retain(&(current->root));
        free(current);
        current = next;
    }
    gc = NULL;
}


Tree *tree_insert(int value, Tree *root, Colletor c) {
    if (tree_search(value, root)) {
        return root;
    }
    else {
        return c(value, root);
    }
}
