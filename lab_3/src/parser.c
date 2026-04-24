#include "parser.h"

#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>

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

/* Добавляет токен в динамический массив */
static int append_token(Token **tokens, int *count, int *capacity, Token token)
{
    Token *new_tokens;
    int new_capacity;

    if (*count < *capacity)
    {
        // Если места хватает, просто записываем токен в конец
        (*tokens)[(*count)++] = token;
        return 1;
    }

    // Если массив заполнен, увеличиваем его размер в 2 раза
    new_capacity = (*capacity == 0) ? 16 : (*capacity * 2);
    new_tokens = realloc(*tokens, new_capacity * sizeof(Token));
    if (new_tokens == NULL)
    {
        return 0;
    }

    *tokens = new_tokens;     // сохраняем новый указатель на массив
    *capacity = new_capacity; // сохраняем новый размер массива
    (*tokens)[(*count)++] = token;
    return 1;
}

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

/* Преобразует входную строку в динамический массив токенов */
static int tokenize(const char *input, Token **tokens_out, int *count_out)
{
    Token *tokens = NULL;
    int count = 0;
    int capacity = 0;
    int pos = 0;

    if (input == NULL || tokens_out == NULL || count_out == NULL)
    {
        return -1;
    }

    while (input[pos] != '\0')
    {
        Token token;

        if (isspace((unsigned char)input[pos]))
        {
            pos++; // пропускаем любые пробельные символы
            continue;
        }

        if (isdigit((unsigned char)input[pos]))
        {
            long long value = 0;

            // Читаем все подряд идущие цифры
            while (isdigit((unsigned char)input[pos]))
            {
                value = value * 10 + (input[pos] - '0'); // из символа в число например '3' - '0' = 3
                pos++;
            }

            token.type = TOKEN_NUMBER;
            token.data.number = value;
        }
        else if (isalpha((unsigned char)input[pos]))
        {
            token.type = TOKEN_VARIABLE;
            token.data.variable = input[pos];
            pos++;
        }
        else if (is_operator_char(input[pos]))
        {
            // парсим оператор
            token.type = TOKEN_OPERATOR;
            token.data.operator = input[pos];
            pos++;
        }
        else if (input[pos] == '(')
        {
            token.type = TOKEN_LPAREN;
            pos++;
        }
        else if (input[pos] == ')')
        {
            token.type = TOKEN_RPAREN;
            pos++;
        }
        else
        {
            // недопустимый символ
            free(tokens);
            return -1; // ошибка
        }

        if (!append_token(&tokens, &count, &capacity, token))
        {
            free(tokens);
            return -1;
        }
    }

    *tokens_out = tokens;
    *count_out = count;
    return 0;
}

/* Переводит токены из инфиксной формы в ОПН */
static int infix_to_rpn(const Token *tokens, int token_count, Token **output_out, int *output_count_out)
{
    Token *output;
    Token *stack;
    int stack_size = 0;
    int output_count = 0;
    int i;

    if (tokens == NULL || output_out == NULL || output_count_out == NULL || token_count <= 0)
    {
        return -1;
    }

    // для ОПН и стека достаточно буферов размером с число входных токенов
    output = malloc(token_count * sizeof(Token));
    stack = malloc(token_count * sizeof(Token));
    if (output == NULL || stack == NULL)
    {
        free(output);
        free(stack);
        return -1;
    }

    for (i = 0; i < token_count; i++)
    {
        Token current = tokens[i];

        if (current.type == TOKEN_NUMBER || current.type == TOKEN_VARIABLE)
        {
            // Числа и переменные сразу попадают в выходную последовательность
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

                output[output_count++] = stack[--stack_size];
            }

            if (!found_lparen)
            {
                free(output);
                free(stack);
                return -1; // закрывающая скобка без открывающей
            }
        }
        else
        {
            free(output);
            free(stack);
            return -1;
        }
    }

    // После конца входа выгружаем оставшиеся операторы
    while (stack_size > 0)
    {
        if (stack[stack_size - 1].type == TOKEN_LPAREN ||
            stack[stack_size - 1].type == TOKEN_RPAREN)
        {
            free(output);
            free(stack);
            return -1; // скобки не согласованы
        }

        output[output_count++] = stack[--stack_size];
    }

    free(stack);
    // Возвращаем готовую ОПН и ее длину
    *output_out = output;
    *output_count_out = output_count;
    return 0;
}

/* Строит дерево выражения по токенам в ОПН */
static ASTNode *build_ast_from_rpn(const Token *rpn_tokens, int count)
{
    ASTNode **stack;
    int stack_size = 0;
    int i;
    ASTNode *result;

    if (rpn_tokens == NULL || count <= 0)
    {
        return NULL;
    }

    // В худшем случае стек узлов по размеру равен числу токенов ОПН
    stack = malloc(count * sizeof(ASTNode *));
    if (stack == NULL)
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
                free(stack);
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
                free(stack);
                return NULL;
            }

            stack[stack_size++] = node;
        }
        else if (rpn_tokens[i].type == TOKEN_OPERATOR)
        {
            ASTNode *right;
            ASTNode *left;

            // Для бинарного оператора нужно два операнда в стеке
            if (stack_size < 2)
            {
                while (stack_size > 0)
                {
                    ast_free(stack[--stack_size]);
                }
                free(stack);
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
                free(stack);
                return NULL;
            }

            // Новый операторный узел снова кладем в стек
            stack[stack_size++] = node;
        }
        else
        {
            // Если токен некорректен, очищаем уже построенную часть дерева
            while (stack_size > 0)
            {
                ast_free(stack[--stack_size]);
            }
            free(stack);
            return NULL;
        }
    }

    if (stack_size != 1)
    {
        while (stack_size > 0)
        {
            ast_free(stack[--stack_size]);
        }
        free(stack);
        return NULL;
    }

    result = stack[0];
    free(stack);
    return result;
}

/* Строит дерево выражения из входной строки */
ASTNode *parse_expression(const char *input)
{
    Token *tokens = NULL;
    Token *rpn_tokens = NULL;
    ASTNode *root;
    int token_count;
    int rpn_count;

    if (input == NULL)
    {
        return NULL;
    }

    // Сначала разбиваем строку на токены
    if (tokenize(input, &tokens, &token_count) != 0 || token_count <= 0)
    {
        free(tokens);
        return NULL;
    }

    // Затем переводим токены в обратную польскую нотацию
    if (infix_to_rpn(tokens, token_count, &rpn_tokens, &rpn_count) != 0 || rpn_count <= 0)
    {
        free(tokens);
        free(rpn_tokens);
        return NULL;
    }

    // По готовой ОПН строим дерево выражения
    root = build_ast_from_rpn(rpn_tokens, rpn_count);

    // Временные массивы токенов больше не нужны
    free(tokens);
    free(rpn_tokens);
    return root;
}
