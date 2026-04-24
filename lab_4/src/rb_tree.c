#include "rb_tree.h"

#include <stdlib.h> // malloc, free
#include <string.h>

/* Проверяет, что символ является латинской буквой */
static int rb_is_latin_letter(char symbol)
{
    return (symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z');
}

/* Рекурсивно очищает все настоящие узлы */
static void rb_free_node(RBTree *tree, RBNode *node)
{
    if (node == tree->nil)
    {
        return;
    }

    rb_free_node(tree, node->left);
    rb_free_node(tree, node->right);
    free(node);
}

/* Создает пустое дерево со служебным черным nil-узлом */
RBTree *rb_tree_create(void)
{
    RBTree *tree = malloc(sizeof(RBTree));

    if (tree == NULL)
    {
        return NULL;
    }

    tree->nil = malloc(sizeof(RBNode));

    if (tree->nil == NULL)
    {
        free(tree);
        return NULL;
    }

    tree->nil->color = BLACK;
    tree->nil->left = tree->nil;
    tree->nil->right = tree->nil;
    tree->nil->parent = tree->nil;

    tree->root = tree->nil;

    return tree;
}

/* Освобождает дерево вместе со всеми узлами */
void rb_tree_free(RBTree *tree)
{
    if (tree == NULL)
    {
        return;
    }

    rb_free_node(tree, tree->root);
    free(tree->nil);
    free(tree);
}

/* Проверяет ключ: только латинские буквы, длина от 1 до 6 */
int rb_is_valid_key(const char *key)
{
    int i;

    if (key == NULL || key[0] == '\0')
    {
        return 0;
    }

    for (i = 0; key[i] != '\0'; i++)
    {
        if (i >= MAX_KEY_LEN || !rb_is_latin_letter(key[i]))
        {
            return 0;
        }
    }

    return 1;
}

/* Ищет узел по ключу */
RBNode *rb_tree_find(RBTree *tree, const char *key)
{
    RBNode *current;
    int cmp;

    if (tree == NULL || !rb_is_valid_key(key))
    {
        return NULL;
    }

    current = tree->root;

    while (current != tree->nil)
    {
        cmp = strcmp(key, current->key);

        if (cmp == 0)
        {
            return current;
        }

        if (cmp < 0)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }

    return NULL;
}

/* Создает новый красный узел */
static RBNode *rb_create_node(RBTree *tree, const char *key, double value)
{
    RBNode *node = malloc(sizeof(RBNode));

    if (node == NULL)
    {
        return NULL;
    }

    strcpy(node->key, key);
    node->value = value;
    node->color = RED;

    node->left = tree->nil;
    node->right = tree->nil;
    node->parent = tree->nil;

    return node;
}

/* Делает левый поворот вокруг node */
static void rb_rotate_left(RBTree *tree, RBNode *node)
{
    RBNode *right_child = node->right;

    node->right = right_child->left;

    if (right_child->left != tree->nil)
    {
        right_child->left->parent = node;
    }

    right_child->parent = node->parent;

    if (node->parent == tree->nil)
    {
        tree->root = right_child;
    }
    else if (node == node->parent->left)
    {
        node->parent->left = right_child;
    }
    else
    {
        node->parent->right = right_child;
    }

    right_child->left = node;
    node->parent = right_child;
}

/* Делает правый поворот вокруг node */
static void rb_rotate_right(RBTree *tree, RBNode *node)
{
    RBNode *left_child = node->left;

    node->left = left_child->right;

    if (left_child->right != tree->nil)
    {
        left_child->right->parent = node;
    }

    left_child->parent = node->parent;

    if (node->parent == tree->nil)
    {
        tree->root = left_child;
    }
    else if (node == node->parent->right)
    {
        node->parent->right = left_child;
    }
    else
    {
        node->parent->left = left_child;
    }

    left_child->right = node;
    node->parent = left_child;
}

/* Балансировка после вставки */
static void rb_insert_fixup(RBTree *tree, RBNode *node)
{
    RBNode *uncle;
    // если node станет корнем его родитель tree->nil, а nil черный
    while (node->parent->color == RED)
    {
        // отец слева от деда
        if (node->parent == node->parent->parent->left)
        {
            uncle = node->parent->parent->right;

            if (uncle->color == RED)
            {
                // родитель и дядя красные
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            }
            else
            {
                if (node == node->parent->right)
                {
                    // превращаем треугольник в линию
                    node = node->parent;
                    rb_rotate_left(tree, node);
                }

                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rb_rotate_right(tree, node->parent->parent);
            }
        }
        // иначе отец справа от деда
        else
        {
            uncle = node->parent->parent->left;

            if (uncle->color == RED)
            {
                // симметричный случай с красным дядей
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            }
            else
            {
                if (node == node->parent->left)
                {
                    // превращаем треугольник в линию
                    node = node->parent;
                    rb_rotate_right(tree, node);
                }

                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rb_rotate_left(tree, node->parent->parent);
            }
        }
    }

    tree->root->color = BLACK;
}

/* Добавляет новый ключ или обновляет значение существующего */
int rb_tree_insert(RBTree *tree, const char *key, double value)
{
    RBNode *parent;
    RBNode *current;
    RBNode *node;
    int cmp;

    if (tree == NULL || !rb_is_valid_key(key))
    {
        return 0;
    }

    parent = tree->nil;
    current = tree->root;

    // идем вниз по дереву и запоминаем будущего родителя
    while (current != tree->nil)
    {
        cmp = strcmp(key, current->key);

        if (cmp == 0)
        {
            // если уже есть такой ключ, то обновляем его значение
            current->value = value;
            return 1;
        }

        parent = current;

        if (cmp < 0)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }

    node = rb_create_node(tree, key, value);

    if (node == NULL)
    {
        return 0;
    }

    node->parent = parent;

    if (parent == tree->nil)
    {
        tree->root = node;
    }
    else if (strcmp(key, parent->key) < 0)
    {
        parent->left = node;
    }
    else
    {
        parent->right = node;
    }

    rb_insert_fixup(tree, node);

    return 1;
}

/* Печатает поддерево с отступами */
static void rb_print_node(RBTree *tree, RBNode *node, FILE *out, int depth)
{
    int i;

    if (node == tree->nil)
    {
        return;
    }

    rb_print_node(tree, node->right, out, depth + 1);

    for (i = 0; i < depth; i++)
    {
        fprintf(out, "    ");
    }

    fprintf(out, "(%c) %s %g\n", node->color == RED ? 'R' : 'B', node->key, node->value);

    rb_print_node(tree, node->left, out, depth + 1);
}

/* Печатает дерево */
void rb_tree_print(RBTree *tree, FILE *out)
{
    if (tree == NULL || out == NULL)
    {
        return;
    }

    if (tree->root == tree->nil)
    {
        fprintf(out, "Дерево пустое\n");
        return;
    }

    rb_print_node(tree, tree->root, out, 0);
}
