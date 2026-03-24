#include "parser.h"

#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>

#define MAX_TOKENS 256

typedef enum
{
    TOKEN_NUMBER,   // число
    TOKEN_VARIABLE, // переменная
    TOKEN_OPERATOR, // оператор: +, -, *, /, ^
    TOKEN_LPAREN,   // левая скобка
    TOKEN_RPAREN,   // правая скобка
} TokenType;

typedef struct
{
    TokenType type;
    union
    {
        long long number; // для TOKEN_NUMBER
        char variable;    // для TOKEN_VARIABLE
        char operator;    // для TOKEN_OPERATOR
    } data;
} Token;

static int is_operator_char(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

static OperatorType char_to_operator(char c)
{
    switch (c)
    {
    case '+':
        return OP_ADD;
    case '-':
        return OP_SUBTRACT;
    case '*':
        return OP_MULTIPLY;
    case '/':
        return OP_DIVIDE;
    case '^':
        return OP_POWER;
    default:
        return -1; // недопустимый оператор
    }
}

static int get_priority(OperatorType op)
{
    switch (op)
    {
    case OP_ADD:
    case OP_SUBTRACT:
        return 1;
    case OP_MULTIPLY:
    case OP_DIVIDE:
        return 2;
    case OP_POWER:
        return 3;
    default:
        return -1; // недопустимый оператор
    }
}

static int is_right_associative(OperatorType op)
{
    return op == OP_POWER; // только оператор ^ является правоассоциативным
}

/* Преобразует входную строку в массив токенов */
static int tokenize(const char *input, Token *tokens, int max_tokens)
{
    int pos = 0;
    int token_count = 0;

    if (input == NULL || tokens == NULL || max_tokens <= 0)
    {
        return -1;
    }

    while (input[pos] != '\0')
    {
        if (isspace((unsigned char)input[pos]))
        {
            pos++; // пропускаем любые пробельные символы
            continue;
        }

        if (token_count >= max_tokens)
        {
            return -1; // не хватило места в массиве токенов
        }

        if (isdigit((unsigned char)input[pos]))
        {
            long long value = 0;

            // Читаем все подряд идущие цифры
            while (isdigit((unsigned char)input[pos]))
            {
                value = value * 10 + (input[pos] - '0'); // из символа в число наприемр '3' - '0' = 3
                pos++;
            }

            tokens[token_count].type = TOKEN_NUMBER;
            tokens[token_count].data.number = value;
            token_count++;
        }
        else if (isalpha((unsigned char)input[pos]))
        {
            tokens[token_count].type = TOKEN_VARIABLE;
            tokens[token_count].data.variable = input[pos];
            pos++;
            token_count++;
        }
        else if (is_operator_char(input[pos]))
        {
            // парсим оператор
            tokens[token_count].type = TOKEN_OPERATOR;
            tokens[token_count].data.operator = input[pos];
            pos++;
            token_count++;
        }
        else if (input[pos] == '(')
        {
            tokens[token_count].type = TOKEN_LPAREN;
            pos++;
            token_count++;
        }
        else if (input[pos] == ')')
        {
            tokens[token_count].type = TOKEN_RPAREN;
            pos++;
            token_count++;
        }
        else
        {
            // недопустимый символ
            return -1; // ошибка
        }
    }

    return token_count; // количество токенов
}

/* Переводит токены из инфиксной формы в ОПН */
static int infix_to_rpn(const Token *tokens, int token_count, Token *output, int max_output)
{
    Token stack[max_output > 0 ? max_output : 1];
    int stack_size = 0;
    int output_count = 0;
    int i;

    if (tokens == NULL || output == NULL || token_count < 0 || max_output <= 0)
    {
        return -1;
    }

    for (i = 0; i < token_count; i++)
    {
        Token current = tokens[i];

        if (current.type == TOKEN_NUMBER || current.type == TOKEN_VARIABLE)
        {
            // Числа и переменные сразу попадают в выходную последовательность
            if (output_count >= max_output)
            {
                return -1;
            }

            output[output_count++] = current;
        }
        else if (current.type == TOKEN_OPERATOR)
        {
            OperatorType current_op = char_to_operator(current.data.operator);

            // Сравниваем текущий оператор с операторами на вершине стека
            while (stack_size > 0 && stack[stack_size - 1].type == TOKEN_OPERATOR)
            {
                OperatorType top_op = char_to_operator(stack[stack_size - 1].data.operator);
                int current_priority = get_priority(current_op);
                int top_priority = get_priority(top_op);

                if (top_priority > current_priority ||
                    (top_priority == current_priority && !is_right_associative(current_op)))
                {
                    if (output_count >= max_output)
                    {
                        return -1;
                    }

                    // Операторы с большим приоритетом отправляем в выход раньше
                    output[output_count++] = stack[--stack_size];
                }
                else
                {
                    break;
                }
            }

            stack[stack_size++] = current;
        }
        else if (current.type == TOKEN_LPAREN)
        {
            // Открывающую скобку просто кладем в стек
            stack[stack_size++] = current;
        }
        else if (current.type == TOKEN_RPAREN)
        {
            int found_lparen = 0;

            // Выгружаем операторы до ближайшей открывающей скобки
            while (stack_size > 0)
            {
                if (stack[stack_size - 1].type == TOKEN_LPAREN)
                {
                    stack_size--;
                    found_lparen = 1;
                    break;
                }

                if (output_count >= max_output)
                {
                    return -1;
                }

                output[output_count++] = stack[--stack_size];
            }

            if (!found_lparen)
            {
                return -1; // закрывающая скобка без открывающей
            }
        }
        else
        {
            return -1;
        }
    }

    // После конца входа выгружаем оставшиеся операторы
    while (stack_size > 0)
    {
        if (stack[stack_size - 1].type == TOKEN_LPAREN ||
            stack[stack_size - 1].type == TOKEN_RPAREN)
        {
            return -1; // скобки не согласованы
        }

        if (output_count >= max_output)
        {
            return -1;
        }

        output[output_count++] = stack[--stack_size];
    }

    return output_count;
}

/* Строит дерево выражения по токенам в ОПН */
static ASTNode *build_ast_from_rpn(const Token *rpn_tokens, int count)
{
    ASTNode *stack[MAX_TOKENS];
    int stack_size = 0;
    int i;

    if (rpn_tokens == NULL || count <= 0)
    {
        return NULL;
    }

    if (count > MAX_TOKENS)
    {
        return NULL;
    }

    for (i = 0; i < count; i++)
    {
        ASTNode *node = NULL;

        if (rpn_tokens[i].type == TOKEN_NUMBER)
        {
            node = ast_create_number(rpn_tokens[i].data.number);
            if (node == NULL)
            {
                while (stack_size > 0)
                {
                    ast_free(stack[--stack_size]);
                }
                return NULL;
            }

            stack[stack_size++] = node;
        }
        else if (rpn_tokens[i].type == TOKEN_VARIABLE)
        {
            node = ast_create_variable(rpn_tokens[i].data.variable);
            if (node == NULL)
            {
                while (stack_size > 0)
                {
                    ast_free(stack[--stack_size]);
                }
                return NULL;
            }

            stack[stack_size++] = node;
        }
        else if (rpn_tokens[i].type == TOKEN_OPERATOR)
        {
            ASTNode *right;
            ASTNode *left;

            // Для бинарного оператора нужно два операнда в стеке.
            if (stack_size < 2)
            {
                while (stack_size > 0)
                {
                    ast_free(stack[--stack_size]);
                }
                return NULL;
            }

            right = stack[--stack_size];
            left = stack[--stack_size];

            node = ast_create_operator(char_to_operator(rpn_tokens[i].data.operator), left, right);
            if (node == NULL)
            {
                ast_free(left);
                ast_free(right);
                while (stack_size > 0)
                {
                    ast_free(stack[--stack_size]);
                }
                return NULL;
            }

            // Новый операторный узел снова кладем в стек.
            stack[stack_size++] = node;
        }
        else
        {
            while (stack_size > 0)
            {
                ast_free(stack[--stack_size]);
            }
            return NULL;
        }
    }

    if (stack_size != 1)
    {
        while (stack_size > 0)
        {
            ast_free(stack[--stack_size]);
        }
        return NULL;
    }

    return stack[0];
}

/* Строит дерево выражения из входной строки */
ASTNode *parse_expression(const char *input)
{
    Token tokens[MAX_TOKENS];
    Token rpn_tokens[MAX_TOKENS];
    int token_count;
    int rpn_count;

    if (input == NULL)
    {
        return NULL;
    }

    // Сначала разбиваем строку на токены
    token_count = tokenize(input, tokens, MAX_TOKENS);
    if (token_count <= 0)
    {
        return NULL;
    }

    // Затем переводим токены в обратную польскую нотацию
    rpn_count = infix_to_rpn(tokens, token_count, rpn_tokens, MAX_TOKENS);
    if (rpn_count <= 0)
    {
        return NULL;
    }

    // По готовой ОПН строим дерево выражения
    return build_ast_from_rpn(rpn_tokens, rpn_count);
}
