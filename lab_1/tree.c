#include "tree.h"

#include <stdlib.h>

TreeStatus init_tree(Tree *tree)
{
    if (tree == NULL)
    {
        return TREE_INVALID_ARG;
    }

    tree->root = NULL;
    return TREE_OK;
}

static Node *create_node(int value, Node *parent)
{
    Node *new_node = malloc(sizeof(Node));

    if (new_node == NULL)
    {
        return NULL;
    }

    new_node->value = value;
    new_node->parent = parent;
    new_node->first_child = NULL;
    new_node->next_sibling = NULL;

    return new_node;
}

TreeStatus set_root(Tree *tree, int root_value)
{
    if (tree == NULL)
    {
        return TREE_INVALID_ARG;
    }

    if (tree->root != NULL)
    {
        return TREE_ROOT_EXISTS;
    }

    Node *new_node = create_node(root_value, NULL);

    if (new_node == NULL)
    {
        return TREE_ALLOC_FAILED;
    }

    tree->root = new_node;
    return TREE_OK;
}

Node *find_node(Node *current, int value)
{
    if (current == NULL)
    {
        return NULL;
    }

    if (current->value == value)
    {
        return current;
    }

    Node *found = find_node(current->first_child, value); // ищем в поддереве первого ребенка
    if (found != NULL)
    {
        return found;
    }

    return find_node(current->next_sibling, value); // ищем в поддереве следующего брата
}

TreeStatus add_child(Tree *tree, Node *parent, int child_value)
{
    if (tree == NULL || parent == NULL)
    {
        return TREE_INVALID_ARG;
    }

    if (tree->root == NULL)
    {
        return TREE_EMPTY;
    }

    if (find_node(tree->root, parent->value) == NULL)
    {
        return TREE_NOT_FOUND;
    }

    if (find_node(tree->root, child_value) != NULL)
    {
        return TREE_DUPLICATE;
    }

    Node *new_node = create_node(child_value, parent);

    if (new_node == NULL)
    {
        return TREE_ALLOC_FAILED;
    }

    if (parent->first_child == NULL)
    {
        parent->first_child = new_node;
    }
    else
    {
        Node *sibling = parent->first_child;

        while (sibling->next_sibling != NULL)
        {
            sibling = sibling->next_sibling;
        }
        sibling->next_sibling = new_node; // Добавляем нового ребенка в конец списка братьев
    }

    return TREE_OK;
}