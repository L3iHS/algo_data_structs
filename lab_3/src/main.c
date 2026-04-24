#include "ast.h"
#include "parser.h"
#include "print.h"
#include "transform.h"

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char *input = NULL;
    size_t input_size = 0;
    ssize_t read_result;
    ASTNode *root;

    printf("Введите выражение: ");
    read_result = getline(&input, &input_size, stdin);
    if (read_result == -1)
    {
        fprintf(stderr, "Ошибка чтения выражения\n");
        return 1;
    }

    if (read_result > 0 && input[read_result - 1] == '\n')
    {
        input[read_result - 1] = '\0';
    }

    root = parse_expression(input);
    if (root == NULL)
    {
        free(input);
        fprintf(stderr, "Ошибка разбора выражения\n");
        return 1;
    }

    printf("\nИсходное дерево:\n");
    print_tree(root, 0);

    root = transform_expression(root);

    if (root->type != NODE_NUMBER)
    {
        printf("\nУпрощенное дерево:\n");
        print_tree(root, 0);
    }

    printf("\nРезультат: ");
    print_expression(root, stdout);
    printf("\n");

    ast_free(root);
    free(input);
    return 0;
}
