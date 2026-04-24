#include "rb_tree.h"

#include <stdio.h>
#include <stdlib.h>

/* Выполняет одну команду из входного файла */
static void process_command(RBTree *tree, FILE *out, int command, FILE *input)
{
    char key[MAX_KEY_LEN + 2]; // +2 чтобы безопасно прочитать неверный ключ и выдать потом ошибку
    double value;
    RBNode *found;

    if (command == 1)
    {
        if (fscanf(input, "%7s %lf", key, &value) != 2)
        {
            fprintf(out, "Ошибка: неверные параметры добавления\n");
            return;
        }

        if (rb_tree_insert(tree, key, value))
        {
            fprintf(out, "Добавлено: %s %g\n", key, value);
        }
        else
        {
            fprintf(out, "Ошибка: не удалось добавить ключ %s\n", key);
        }
    }
    else if (command == 2)
    {
        if (fscanf(input, "%7s", key) != 1)
        {
            fprintf(out, "Ошибка: неверные параметры удаления\n");
            return;
        }

        if (rb_tree_delete(tree, key))
        {
            fprintf(out, "Удалено: %s\n", key);
        }
        else
        {
            fprintf(out, "Ошибка: ключ %s не найден\n", key);
        }
    }
    else if (command == 3)
    {
        fprintf(out, "Дерево:\n");
        rb_tree_print(tree, out);
    }
    else if (command == 4)
    {
        if (fscanf(input, "%7s", key) != 1)
        {
            fprintf(out, "Ошибка: неверные параметры поиска\n");
            return;
        }

        found = rb_tree_find(tree, key);

        if (found == NULL)
        {
            fprintf(out, "Ключ %s не найден\n", key);
        }
        else
        {
            fprintf(out, "Найдено: %s %g\n", found->key, found->value);
        }
    }
    else
    {
        fprintf(out, "Ошибка: неизвестная команда %d\n", command);
    }
}

int main(int argc, char **argv)
{
    RBTree *tree;
    FILE *input;
    FILE *output;
    int command;

    if (argc < 2 || argc > 3)
    {
        printf("Использование: %s input.txt [output.txt]\n", argv[0]);
        return 1;
    }

    input = fopen(argv[1], "r");

    if (input == NULL)
    {
        printf("Ошибка: не удалось открыть входной файл\n");
        return 1;
    }

    output = stdout;

    if (argc == 3)
    {
        output = fopen(argv[2], "w");

        if (output == NULL)
        {
            printf("Ошибка: не удалось открыть выходной файл\n");
            fclose(input);
            return 1;
        }
    }

    tree = rb_tree_create();

    if (tree == NULL)
    {
        fprintf(output, "Ошибка: не удалось создать дерево\n");
        fclose(input);

        if (output != stdout)
        {
            fclose(output);
        }

        return 1;
    }

    while (fscanf(input, "%d", &command) == 1)
    {
        process_command(tree, output, command, input);
    }

    rb_tree_free(tree);
    fclose(input);

    if (output != stdout)
    {
        fclose(output);
    }

    return 0;
}
