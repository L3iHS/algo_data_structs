#include "tree.h"

#include <stdio.h>
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

void delete_subtree(Node *node)
{
    if (node == NULL)
    {
        return;
    }

    Node *child = node->first_child;

    while (child != NULL)
    {
        Node *next_child = child->next_sibling;
        delete_subtree(child);
        child = next_child;
    }

    free(node);
}

TreeStatus free_tree(Tree *tree)
{
    if (tree == NULL)
    {
        return TREE_INVALID_ARG;
    }
    if (tree->root == NULL)
    {
        return TREE_OK;
    }

    delete_subtree(tree->root);
    tree->root = NULL;
    return TREE_OK;
}

TreeStatus remove_node(Tree *tree, int value)
{
    if (tree == NULL)
    {
        return TREE_INVALID_ARG;
    }
    if (tree->root == NULL)
    {
        return TREE_EMPTY;
    }
    Node *target = find_node(tree->root, value);

    if (target == NULL)
    {
        return TREE_NOT_FOUND;
    }

    if (target == tree->root)
    {
        delete_subtree(tree->root);
        tree->root = NULL;
        return TREE_OK;
    }

    if (target == target->parent->first_child)
    {
        target->parent->first_child = target->next_sibling;
    }
    else
    {
        Node *sibling = target->parent->first_child;

        while (sibling != NULL && sibling->next_sibling != target)
        {
            sibling = sibling->next_sibling;
        }

        if (sibling != NULL)
        {
            sibling->next_sibling = target->next_sibling;
        }
    }

    target->next_sibling = NULL;
    delete_subtree(target);
    return TREE_OK;
}

static void print_tree_recursive(const Node *node, int depth)
{
    if (node == NULL)
    {
        return;
    }
    for (int i = 0; i < depth; i++)
    {
        printf("  ");
    }
    printf("%d\n", node->value);
    print_tree_recursive(node->first_child, depth + 1);
    print_tree_recursive(node->next_sibling, depth);
}

TreeStatus print_tree(const Node *node)
{
    if (node == NULL)
    {
        return TREE_EMPTY;
    }
    print_tree_recursive(node, 0);
    return TREE_OK;
}

static TreeStatus helper_find_deepest_nonterminal(const Node *current, int current_depth, int *best_depth, int *best_value, int *found)
{
    if (current == NULL)
    {
        return TREE_OK;
    }

    if (current->first_child != NULL) // Нетерминальная вершина
    {
        if (current_depth > *best_depth)
        {
            *best_depth = current_depth;
            *best_value = current->value;
            *found = 1;
        }
    }

    TreeStatus status = helper_find_deepest_nonterminal(current->first_child, current_depth + 1, best_depth, best_value, found);
    if (status != TREE_OK)
    {
        return status;
    }

    return helper_find_deepest_nonterminal(current->next_sibling, current_depth, best_depth, best_value, found);
}

TreeStatus find_deepest_nonterminal(const Node *node, int *out_value)
{
    if (out_value == NULL)
    {
        return TREE_INVALID_ARG;
    }

    if (node == NULL)
    {
        return TREE_NO_NONTERMINAL;
    }

    int best_depth = -1;
    int best_value = 0;
    int found = 0;

    TreeStatus status = helper_find_deepest_nonterminal(node, 0, &best_depth, &best_value, &found);

    if (status != TREE_OK)
    {
        return status;
    }
    
    if (found == 0)
    {
        return TREE_NO_NONTERMINAL;
    }
    else
    {
        *out_value = best_value;
    }

    return TREE_OK;
}