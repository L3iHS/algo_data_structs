#include "print.h"

/* Возвращает символ для заданной операции */
static char operator_to_char(OperatorType op)
{
    switch (op)
    {
    case OP_ADD:
        return '+';
    case OP_SUBTRACT:
        return '-';
    case OP_MULTIPLY:
        return '*';
    case OP_DIVIDE:
        return '/';
    case OP_POWER:
        return '^';
    default:
        return '?';
    }
}

/* Печатает дерево выражения с отступами по глубине */
void print_tree(const ASTNode *node, int depth)
{
    int i;

    if (node == NULL)
    {
        return;
    }

    for (i = 0; i < depth; i++)
    {
        printf("    ");
    }

    if (node->type == NODE_NUMBER)
    {
        printf("%lld\n", node->data.number);
    }
    else if (node->type == NODE_VARIABLE)
    {
        printf("%c\n", node->data.variable);
    }
    else if (node->type == NODE_OPERATOR)
    {
        printf("%c\n", operator_to_char(node->data.operator_data.op));
        print_tree(node->data.operator_data.left, depth + 1);
        print_tree(node->data.operator_data.right, depth + 1);
    }
}

/* Печатает дерево в обычной записи */
void print_expression(const ASTNode *node, FILE *out)
{
    if (node == NULL || out == NULL)
    {
        return;
    }

    if (node->type == NODE_NUMBER)
    {
        fprintf(out, "%lld", node->data.number);
    }
    else if (node->type == NODE_VARIABLE)
    {
        fprintf(out, "%c", node->data.variable);
    }
    else if (node->type == NODE_OPERATOR)
    {
        fprintf(out, "(");
        print_expression(node->data.operator_data.left, out);
        fprintf(out, " %c ", operator_to_char(node->data.operator_data.op));
        print_expression(node->data.operator_data.right, out);
        fprintf(out, ")");
    }
}
