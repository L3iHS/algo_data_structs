#ifndef RB_TREE_H
#define RB_TREE_H

#include <stdio.h>

#define MAX_KEY_LEN 6

typedef enum
{
    RED,
    BLACK
} RBColor;

typedef struct RBNode
{
    char key[MAX_KEY_LEN + 1];
    double value;
    RBColor color;

    struct RBNode *left;
    struct RBNode *right;
    struct RBNode *parent;
} RBNode;

typedef struct
{
    RBNode *root;
    RBNode *nil;
} RBTree;

/* Создание и удаление дерева */
RBTree *rb_tree_create(void);
void rb_tree_free(RBTree *tree);

/* Основные операции */
int rb_tree_insert(RBTree *tree, const char *key, double value);
int rb_tree_delete(RBTree *tree, const char *key);
RBNode *rb_tree_find(RBTree *tree, const char *key);

/* Печать дерева */
void rb_tree_print(RBTree *tree, FILE *out);

/* Проверка ключа: только латинские буквы, длина от 1 до 6 */
int rb_is_valid_key(const char *key);

#endif