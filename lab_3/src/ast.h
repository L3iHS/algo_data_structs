#ifndef AST_H
#define AST_H

// ast - абстрактное синтаксическое дерево
typedef enum
{
    NODE_NUMBER,   // число
    NODE_VARIABLE, // переменная, например x
    NODE_OPERATOR  // бинарная операция: +, -, *, /, ^
} NodeType;

typedef enum
{
    OP_ADD,      // +
    OP_SUBTRACT, // -
    OP_MULTIPLY, // *
    OP_DIVIDE,   // /
    OP_POWER     // ^
} OperatorType;

typedef struct ASTNode
{
    NodeType type;
    union // тип, где несколько полей делят одну и ту же память, в зависимости от типа узла
    {
        long long number; // для NODE_NUMBER
        char variable;    // для NODE_VARIABLE
        struct
        {
            OperatorType op;       // тип операции для NODE_OPERATOR
            struct ASTNode *left;  // левое поддерево
            struct ASTNode *right; // правое поддерево
        } operator_data;
    } data;
} ASTNode;

ASTNode *ast_create_number(long long value);
ASTNode *ast_create_variable(char name);
ASTNode *ast_create_operator(OperatorType op, ASTNode *left, ASTNode *right);
void ast_free(ASTNode *node);

#endif