#ifndef TREE_H
#define TREE_H

typedef struct Node
{
    int value;
    struct Node *parent;
    struct Node *first_child;
    struct Node *next_sibling;
} Node;

typedef struct Tree
{
    Node *root;
} Tree;

typedef enum
{
    TREE_OK,            // Все операции прошли успешно
    TREE_INVALID_ARG,   // Неверный аргумент
    TREE_EMPTY,         // Дерево пустое
    TREE_NOT_FOUND,     // Узел не найден
    TREE_DUPLICATE,     // Узел с таким значением уже существует
    TREE_ALLOC_FAILED,  // Ошибка выделения памяти
    TREE_ROOT_EXISTS,   // Корень уже существует
    TREE_NO_NONTERMINAL // Нетерминальных вершин нет
} TreeStatus;

TreeStatus init_tree(Tree *tree);
Node *find_node(Node *current, int value);
TreeStatus set_root(Tree *tree, int root_value);
TreeStatus add_child(Tree *tree, Node *parent, int child_value);
TreeStatus print_tree(const Node *node);
TreeStatus remove_node(Tree *tree, int value);
void delete_subtree(Node *node);
TreeStatus find_deepest_nonterminal(const Node *node, int *out_value);
TreeStatus free_tree(Tree *tree);

#endif