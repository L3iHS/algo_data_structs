#include "../src/rb_tree.h"

#include <math.h>
#include <stdio.h>

static int double_equal(double left, double right)
{
    return fabs(left - right) < 0.000001;
}

/* Проверяет, что ключ найден с нужным значением */
static int test_find_value(RBTree *tree, const char *key, double expected)
{
    RBNode *node = rb_tree_find(tree, key);

    if (node == NULL)
    {
        printf("ОШИБКА: ключ %s не найден\n", key);
        return 0;
    }

    if (!double_equal(node->value, expected))
    {
        printf("ОШИБКА: для ключа %s ожидалось %g, получено %g\n", key, expected, node->value);
        return 0;
    }

    return 1;
}

/* Проверяет добавление и поиск */
static int test_insert_and_find(void)
{
    RBTree *tree = rb_tree_create();
    int result;

    if (tree == NULL)
    {
        printf("ОШИБКА: дерево не создано\n");
        return 0;
    }

    rb_tree_insert(tree, "keya", 1.5);
    rb_tree_insert(tree, "keyb", 2.5);
    rb_tree_insert(tree, "keyc", 3.5);

    result = test_find_value(tree, "keyb", 2.5);

    rb_tree_free(tree);
    return result;
}

/* Проверяет обновление значения по существующему ключу */
static int test_update_value(void)
{
    RBTree *tree = rb_tree_create();
    int result;

    if (tree == NULL)
    {
        printf("ОШИБКА: дерево не создано\n");
        return 0;
    }

    rb_tree_insert(tree, "keya", 1.0);
    rb_tree_insert(tree, "keya", 9.0);

    result = test_find_value(tree, "keya", 9.0);

    rb_tree_free(tree);
    return result;
}

/* Проверяет удаление существующего ключа */
static int test_delete_existing(void)
{
    RBTree *tree = rb_tree_create();
    int result;

    if (tree == NULL)
    {
        printf("ОШИБКА: дерево не создано\n");
        return 0;
    }

    rb_tree_insert(tree, "keyb", 2.0);
    rb_tree_insert(tree, "keya", 1.0);
    rb_tree_insert(tree, "keyc", 3.0);

    result = rb_tree_delete(tree, "keyb") && rb_tree_find(tree, "keyb") == NULL;

    if (!result)
    {
        printf("ОШИБКА: ключ keyb не удалился\n");
    }

    rb_tree_free(tree);
    return result;
}

/* Проверяет, что неверные ключи не добавляются */
static int test_invalid_keys(void)
{
    RBTree *tree = rb_tree_create();
    int result;

    if (tree == NULL)
    {
        printf("ОШИБКА: дерево не создано\n");
        return 0;
    }

    result = !rb_tree_insert(tree, "key_1", 1.0) &&
             !rb_tree_insert(tree, "key123", 2.0) &&
             !rb_tree_insert(tree, "toolong", 3.0) &&
             !rb_tree_insert(tree, "", 4.0);

    if (!result)
    {
        printf("ОШИБКА: неверный ключ был добавлен\n");
    }

    rb_tree_free(tree);
    return result;
}

/* Проверяет работу дерева на примере больше 20 узлов */
static int test_many_nodes(void)
{
    const char *keys[] = {
        "keym", "keya", "keyt", "keyb", "keyc", "keyg", "keyl", "keyp",
        "keys", "keyk", "keyn", "keyd", "keye", "keyf", "keyh", "keyi",
        "keyj", "keyo", "keyq", "keyu", "keyr", "keyv"};
    RBTree *tree = rb_tree_create();
    int i;
    int result;

    if (tree == NULL)
    {
        printf("ОШИБКА: дерево не создано\n");
        return 0;
    }

    for (i = 0; i < 22; i++)
    {
        rb_tree_insert(tree, keys[i], i + 1);
    }

    result = test_find_value(tree, "keya", 2.0) &&
             test_find_value(tree, "keyv", 22.0) &&
             rb_tree_delete(tree, "keym") &&
             rb_tree_find(tree, "keym") == NULL &&
             test_find_value(tree, "keyq", 19.0);

    if (!result)
    {
        printf("ОШИБКА: тест с большим деревом не прошел\n");
    }

    rb_tree_free(tree);
    return result;
}

int main(void)
{
    int passed = 0;
    int total = 0;

    total++;
    passed += test_insert_and_find();

    total++;
    passed += test_update_value();

    total++;
    passed += test_delete_existing();

    total++;
    passed += test_invalid_keys();

    total++;
    passed += test_many_nodes();

    printf("Пройдено %d из %d тестов\n", passed, total);

    return passed == total ? 0 : 1;
}
