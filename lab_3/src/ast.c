#include "ast.h"

#include <stdlib.h>

ASTNode *ast_create_number(long long value)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    if (node == NULL)
        return NULL; // Ошибка выделения памяти

    node->type = NODE_NUMBER;
    node->data.number = value;
    return node;
}

ASTNode *ast_create_variable(char name)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    if (node == NULL)
        return NULL; // Ошибка выделения памяти

    node->type = NODE_VARIABLE;
    node->data.variable = name;
    return node;
}

ASTNode *ast_create_operator(OperatorType op, ASTNode *left, ASTNode *right)
{
    ASTNode *node = malloc(sizeof(ASTNode));
    if (node == NULL)
        return NULL; // Ошибка выделения памяти

    node->type = NODE_OPERATOR;
    node->data.operator_data.op = op;
    node->data.operator_data.left = left;
    node->data.operator_data.right = right;
    return node;
}

void ast_free(ASTNode *node)
{
    if (node == NULL)
        return;

    if (node->type == NODE_OPERATOR)
    // рекурсивно удаляем левое и правое поддерево
    {
        ast_free(node->data.operator_data.left);
        ast_free(node->data.operator_data.right);
    }

    free(node);
}