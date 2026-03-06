#include "tree.h"

#include <stdio.h>

// собрали дерево и проверили, что поиск работает
static int test_add_and_find(void)
{
    Tree tree;
    if (init_tree(&tree) != TREE_OK)
    {
        return 0;
    }
    if (set_root(&tree, 10) != TREE_OK)
    {
        return 0;
    }

    Node *root = tree.root;
    if (add_child(&tree, root, 3) != TREE_OK)
    {
        return 0;
    }
    if (add_child(&tree, root, 8) != TREE_OK)
    {
        return 0;
    }

    Node *node3 = find_node(tree.root, 3);
    if (node3 == NULL)
    {
        return 0;
    }
    if (add_child(&tree, node3, 4) != TREE_OK)
    {
        return 0;
    }

    if (find_node(tree.root, 4) == NULL)
    {
        return 0;
    }

    free_tree(&tree);
    return 1;
}

// повторная вставка того же значения должна дать TREE_DUPLICATE
static int test_duplicate_value(void)
{
    Tree tree;
    if (init_tree(&tree) != TREE_OK)
    {
        return 0;
    }
    if (set_root(&tree, 1) != TREE_OK)
    {
        return 0;
    }

    Node *root = tree.root;
    if (add_child(&tree, root, 2) != TREE_OK)
    {
        return 0;
    }

    int ok = add_child(&tree, root, 2) == TREE_DUPLICATE;
    free_tree(&tree);
    return ok;
}

// при удалении узла с потомками должны исчезнуть оба
static int test_remove_subtree(void)
{
    Tree tree;
    if (init_tree(&tree) != TREE_OK)
    {
        return 0;
    }
    if (set_root(&tree, 1) != TREE_OK)
    {
        return 0;
    }

    Node *root = tree.root;
    if (add_child(&tree, root, 2) != TREE_OK)
    {
        return 0;
    }
    if (add_child(&tree, root, 3) != TREE_OK)
    {
        return 0;
    }

    Node *node2 = find_node(tree.root, 2);
    if (node2 == NULL)
    {
        return 0;
    }
    if (add_child(&tree, node2, 4) != TREE_OK)
    {
        return 0;
    }

    if (remove_node(&tree, 2) != TREE_OK)
    {
        return 0;
    }

    if (find_node(tree.root, 2) != NULL)
    {
        return 0;
    }
    if (find_node(tree.root, 4) != NULL)
    {
        return 0;
    }
    if (find_node(tree.root, 3) == NULL)
    {
        return 0;
    }

    free_tree(&tree);
    return 1;
}

// при равных глубинах берем первую нетерминальную вершину при обходе в глубину слева напраыо 
static int test_deepest_nonterminal_tie(void)
{
    Tree tree;
    if (init_tree(&tree) != TREE_OK)
    {
        return 0;
    }
    if (set_root(&tree, 1) != TREE_OK)
    {
        return 0;
    }

    Node *root = tree.root;
    if (add_child(&tree, root, 2) != TREE_OK)
    {
        return 0;
    }
    if (add_child(&tree, root, 3) != TREE_OK)
    {
        return 0;
    }

    Node *node2 = find_node(tree.root, 2);
    Node *node3 = find_node(tree.root, 3);
    if (node2 == NULL || node3 == NULL)
    {
        return 0;
    }

    if (add_child(&tree, node2, 4) != TREE_OK)
    {
        return 0;
    }
    if (add_child(&tree, node3, 5) != TREE_OK)
    {
        return 0;
    }

    Node *node4 = find_node(tree.root, 4);
    Node *node5 = find_node(tree.root, 5);
    if (node4 == NULL || node5 == NULL)
    {
        return 0;
    }

    if (add_child(&tree, node4, 6) != TREE_OK)
    {
        return 0;
    }
    if (add_child(&tree, node5, 7) != TREE_OK)
    {
        return 0;
    }

    int value = -1;
    TreeStatus status = find_deepest_nonterminal(tree.root, &value);
    free_tree(&tree);

    return status == TREE_OK && value == 4;
}

// отсутвие нетерминальных вершин при пустом и из одного узла дереве
static int test_no_nonterminal(void)
{
    Tree tree;
    if (init_tree(&tree) != TREE_OK)
    {
        return 0;
    }

    int value = -1;
    if (find_deepest_nonterminal(tree.root, &value) != TREE_NO_NONTERMINAL)
    {
        return 0;
    }

    if (set_root(&tree, 100) != TREE_OK)
    {
        return 0;
    }

    if (find_deepest_nonterminal(tree.root, &value) != TREE_NO_NONTERMINAL)
    {
        return 0;
    }

    free_tree(&tree);
    return 1;
}

int main(void)
{
    int passed = 0;
    int total = 5;

    passed += test_add_and_find();
    passed += test_duplicate_value();
    passed += test_remove_subtree();
    passed += test_deepest_nonterminal_tie();
    passed += test_no_nonterminal();

    printf("Passed %d/%d tests\n", passed, total);
    return passed == total ? 0 : 1;
}
