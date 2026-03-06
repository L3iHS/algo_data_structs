#include "tree.h"

#include <stdio.h>

int main(void)
{
    Tree tree;
    init_tree(&tree);

    while (1)
    {

        printf("0 - выход\n");
        printf("1 - добавить узел\n");
        printf("2 - вывести дерево\n");
        printf("3 - удалить узел\n");
        printf("4 - найти глубочайшую нетерминальную вершину\n");
        printf("Введите пункт: ");

        int point;
        if (scanf("%d", &point) != 1)
        {
            printf("Неверный ввод\n");
            while (getchar() != '\n');  // Очистка буфера
            continue;
        }

        switch (point)
        {
        case 0:
            free_tree(&tree);
            return 0;
        case 1:
            if (tree.root == NULL)
            {
                int root_value;
                printf("Введите значение root: ");
                if (scanf("%d", &root_value) == 1)
                {
                    TreeStatus status = set_root(&tree, root_value);
                    switch (status)
                    {
                    case TREE_OK:
                        break;
                    case TREE_INVALID_ARG:
                        printf("Неверный аргумент\n");
                        break;
                    case TREE_ROOT_EXISTS:
                        printf("Корень уже существует\n");
                        break;
                    case TREE_ALLOC_FAILED:
                        printf("Ошибка выделения памяти\n");
                        break;
                    default:
                        printf("Ошибка при установке корня\n");
                        break;
                    }
                }
                else
                {
                    printf("Неверный ввод\n");
                    while (getchar() != '\n');
                    continue;
                }
            }
            else
            {
                printf("Введите значение родителя: \n");
                int parent_value;
                if (scanf("%d", &parent_value) == 1)
                {
                    Node *parent_node = find_node(tree.root, parent_value);
                    if (parent_node == NULL)
                    {
                        printf("Родитель не найден\n");
                        break;
                    }
                    printf("Введите значение ребенка: \n");
                    int child_value;
                    if (scanf("%d", &child_value) == 1)
                    {
                        TreeStatus status = add_child(&tree, parent_node, child_value);
                        switch (status)
                        {
                        case TREE_OK:
                            break;
                        case TREE_INVALID_ARG:
                            printf("Неверный аргумент\n");
                            break;
                        case TREE_EMPTY:
                            printf("Дерево пустое\n");
                            break;
                        case TREE_NOT_FOUND:
                            printf("Родитель не найден\n");
                            break;
                        case TREE_DUPLICATE:
                            printf("Узел с таким значением уже существует\n");
                            break;
                        case TREE_ALLOC_FAILED:
                            printf("Ошибка выделения памяти\n");
                            break;
                        default:
                            printf("Ошибка при добавлении ребенка\n");
                            break;
                        }
                    }
                    else
                    {
                        printf("Неверный ввод\n");
                        while (getchar() != '\n');
                        continue;
                    }
                }
                else
                {
                    printf("Неверный ввод\n");
                    while (getchar() != '\n');
                    continue;
                }
            }
            break;
        case 2:
        {
            TreeStatus status = print_tree(tree.root);
            switch (status)
            {
            case TREE_OK:
                break;
            case TREE_EMPTY:
                printf("Дерево пустое\n");
                break;
            default:
                printf("Ошибка при печати дерева\n");
            }
            break;
        }
        case 3:
        {
            printf("Введите значение для удаления: ");
            int value_to_remove;
            if (scanf("%d", &value_to_remove) == 1)
            {
                TreeStatus status = remove_node(&tree, value_to_remove);
                switch (status)
                {
                case TREE_OK:
                    printf("Узел успешно удален\n");
                    break;
                case TREE_INVALID_ARG:
                    printf("Неверный аргумент\n");
                    break;
                case TREE_EMPTY:
                    printf("Дерево пустое\n");
                    break;
                case TREE_NOT_FOUND:
                    printf("Узел не найден\n");
                    break;
                default:
                    printf("Ошибка при удалении узла\n");
                    break;
                }
            }
            else
            {
                printf("Неверный ввод\n");
                while (getchar() != '\n');
                continue;
            }
            break;
        }
        case 4:
        {
            int result;
            TreeStatus status = find_deepest_nonterminal(tree.root, &result);
            if (status == TREE_OK)
            {
                printf("Глубочайшая нетерминальная вершина: %d\n", result);
            }
            else if (status == TREE_NO_NONTERMINAL)
            {
                printf("Нетерминальных вершин нет\n");
            }
            else
            {
                printf("Ошибка при поиске глубочайшей нетерминальной вершины\n");
            }
            break;
        }
        default:
            printf("Неверный пункт меню\n");
        }
    }
}