#include "ast.h"
#include "parser.h"
#include "print.h"
#include "transform.h"

#include <stdio.h>

#define INPUT_SIZE 512

int main(void)
{
    char input[INPUT_SIZE];
    ASTNode *root;

    printf("Введите выражение: ");
    if (fgets(input, sizeof(input), stdin) == NULL)
    {
        fprintf(stderr, "Ошибка чтения выражения.\n");
        return 1;
    }

    root = parse_expression(input);
    if (root == NULL)
    {
        fprintf(stderr, "Ошибка разбора выражения.\n");
        return 1;
    }

    printf("\nИсходное дерево:\n");
    print_tree(root, 0);

    root = transform_expression(root);

    printf("\nУпрощенное дерево:\n");
    print_tree(root, 0);

    printf("\nРезультат: ");
    print_expression(root, stdout);
    printf("\n");

    ast_free(root);
    return 0;
}
