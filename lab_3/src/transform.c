#include "transform.h"

#include <stddef.h>

/* Проверяет, что узел существует и хранит число */
static int is_number_node(const ASTNode *node)
{
    return node != NULL && node->type == NODE_NUMBER;
}

/* Вычисляет целую степень для неотрицательного показателя */
static long long integer_power(long long base, long long exponent)
{
    long long result = 1;
    long long i;

    for (i = 0; i < exponent; i++)
    {
        result *= base;
    }

    return result;
}

/* Проверяет, можно ли безопасно вычислить операцию */
static int can_evaluate_operator(OperatorType op, long long left, long long right)
{
    switch (op)
    {
    case OP_ADD:
    case OP_SUBTRACT:
    case OP_MULTIPLY:
        return 1;
    case OP_DIVIDE:
        return right != 0 && left % right == 0;
    case OP_POWER:
        return right >= 0;
    default:
        return 0;
    }
}

/* Вычисляет результат бинарной операции над числами */
static long long evaluate_operator(OperatorType op, long long left, long long right)
{
    switch (op)
    {
    case OP_ADD:
        return left + right;
    case OP_SUBTRACT:
        return left - right;
    case OP_MULTIPLY:
        return left * right;
    case OP_DIVIDE:
        return left / right;
    case OP_POWER:
        return integer_power(left, right);
    default:
        return 0;
    }
}

/* Рекурсивно упрощает дерево выражения */
ASTNode *transform_expression(ASTNode *node)
{
    ASTNode *left;
    ASTNode *right;
    OperatorType op;
    long long result;

    if (node == NULL)
    {
        return NULL;
    }

    if (node->type != NODE_OPERATOR)
    {
        return node;
    }

    // упрощаем левое и правое поддерево
    node->data.operator_data.left = transform_expression(node->data.operator_data.left);
    node->data.operator_data.right = transform_expression(node->data.operator_data.right);

    left = node->data.operator_data.left;
    right = node->data.operator_data.right;
    op = node->data.operator_data.op;

    // вычисляем узел если оба потомка уже числа
    if (!is_number_node(left) || !is_number_node(right))
    {
        return node;
    }

    if (!can_evaluate_operator(op, left->data.number, right->data.number))
    {
        return node;
    }

    result = evaluate_operator(op, left->data.number, right->data.number);

    // освобождаем поддеревья и превращаем текущий узел в число
    ast_free(left);
    ast_free(right);

    node->type = NODE_NUMBER;
    node->data.number = result;

    return node;
}
