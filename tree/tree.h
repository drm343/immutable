#ifndef TREE_H
#define TREE_H 1

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


enum _T_Type;
typedef struct _tree Tree;


Tree *tree_node(enum _T_Type type, int value, Tree *left, Tree *right);

Tree *tree_leaf(int value);
#define V(a) tree_leaf(a)

Tree *tree_branch(int value, Tree *left, Tree *right);
#define B(n, a, b) tree_branch(n, a, b)


Tree *tree_root_shared(Tree *root);

const Tree const *tree_left(Tree *root);
const Tree const *tree_left_static(const Tree const *root);
Tree *tree_left_shared(Tree *root);
Tree *tree_left_copy(const Tree const *root);
Tree *tree_left_move(Tree *root);

const Tree const *tree_right(Tree *root);
const Tree const *tree_right_static(const Tree const *root);
Tree *tree_right_shared(Tree *root);
Tree *tree_right_copy(const Tree const *root);
Tree *tree_right_move(Tree *root);


bool tree_is_leaf(Tree *root);
bool tree_is_leaf_static(const Tree const *root);
bool tree_is_node(Tree *root);
bool tree_is_node_static(const Tree const *root);
int tree_value(Tree *root);
int tree_value_static(const Tree const *root);


/* free node */
void tree_free(Tree **t_ptr);

/* use in reference counter */
void tree_retain(Tree **t_ptr);

/* force free all structures */
void tree_release(Tree **t_ptr);

void tree_debug(const Tree const *t);
#define TREE_DEBUG(a) tree_debug(a); printf("\n");


bool tree_search(int value, Tree *root);
typedef Tree *(*Colletor)(int value, Tree *root);
Tree *tree_insert(int value, Tree *root, Colletor c);

Tree *base_insert_colletor(int value, Tree *root);
Tree *base_gc_insert_colletor(int value, Tree *root);

void tree_mark(Tree *root);
void tree_sweep(void);
#endif
