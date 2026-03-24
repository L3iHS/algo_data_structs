#include "../src/ast.h"
#include "../src/parser.h"
#include "../src/transform.h"

#include <stdio.h>

/* Проверяет, что выражение после преобразования стало числом */
static int test_number_result(const char *expression, long long expected)
{
    ASTNode *root = parse_expression(expression);

    // Сначала строим дерево по строке
    if (root == NULL)
    {
        printf("ОШИБКА: parse_expression(\"%s\") вернула NULL\n", expression);
        return 0;
    }

    // Затем упрощаем дерево
    root = transform_expression(root);

    if (root->type != NODE_NUMBER)
    {
        printf("ОШИБКА: выражение \"%s\" не превратилось в число\n", expression);
        ast_free(root);
        return 0;
    }

    if (root->data.number != expected)
    {
        printf("ОШИБКА: для \"%s\" ожидалось %lld, получено %lld\n", expression, expected, root->data.number);
        ast_free(root);
        return 0;
    }

    // Освобождаем память после успешной проверки
    ast_free(root);
    return 1;
}

/* Проверяет, что выражение осталось операторным узлом */
static int test_operator_root(const char *expression, OperatorType expected_op)
{
    ASTNode *root = parse_expression(expression);

    if (root == NULL)
    {
        printf("ОШИБКА: parse_expression(\"%s\") вернула NULL\n", expression);
        return 0;
    }

    // Упрощаем дерево и проверяем, что корень остался оператором
    root = transform_expression(root);

    if (root->type != NODE_OPERATOR)
    {
        printf("ОШИБКА: выражение \"%s\" не осталось оператором\n", expression);
        ast_free(root);
        return 0;
    }

    if (root->data.operator_data.op != expected_op)
    {
        printf("ОШИБКА: у выражения \"%s\" неожиданный оператор в корне\n", expression);
        ast_free(root);
        return 0;
    }

    ast_free(root);
    return 1;
}

/* Проверяет, что выражение осталось степенью с числовым показателем */
static int test_power_with_variable_base(const char *expression, long long expected_exponent)
{
    ASTNode *root = parse_expression(expression);
    ASTNode *left;
    ASTNode *right;

    if (root == NULL)
    {
        printf("ОШИБКА: parse_expression(\"%s\") вернула NULL\n", expression);
        return 0;
    }

    // После преобразования основание должно остаться переменной,
    // а показатель должен свернуться в число
    root = transform_expression(root);

    if (root->type != NODE_OPERATOR || root->data.operator_data.op != OP_POWER)
    {
        printf("ОШИБКА: выражение \"%s\" не осталось степенью\n", expression);
        ast_free(root);
        return 0;
    }

    left = root->data.operator_data.left;
    right = root->data.operator_data.right;

    if (left == NULL || left->type != NODE_VARIABLE)
    {
        printf("ОШИБКА: выражение \"%s\" потеряло переменную в основании\n", expression);
        ast_free(root);
        return 0;
    }

    if (right == NULL || right->type != NODE_NUMBER || right->data.number != expected_exponent)
    {
        printf("ОШИБКА: у выражения \"%s\" неожиданный показатель степени\n", expression);
        ast_free(root);
        return 0;
    }

    ast_free(root);
    return 1;
}

int main(void)
{
    int passed = 0;
    int total = 0;

    // Простая степень
    total++;
    passed += test_number_result("2 ^ 3", 8);

    // Основание и показатель сначала сворачиваются в числа, затем вычисляется степень
    total++;
    passed += test_number_result("(2 + 3) ^ (1 * 7)", 78125);

    // Большой пример с полным вычислением выражения
    total++;
    passed += test_number_result("(2 + (3 + 7 * 2) * 1 * 2) ^ (5 * 3 - 10 - 1) / 2", 839808);

    // Основание не число, поэтому степень должна остаться в дереве
    total++;
    passed += test_power_with_variable_base("x ^ (1 + 2)", 3);

    // Деление с остатком не вычисляем
    total++;
    passed += test_operator_root("7 / 2", OP_DIVIDE);

    // Степень с отрицательным показателем не вычисляем
    total++;
    passed += test_operator_root("2 ^ (3 - 5)", OP_POWER);

    printf("Пройдено %d из %d тестов\n", passed, total);

    return passed == total ? 0 : 1;
}
