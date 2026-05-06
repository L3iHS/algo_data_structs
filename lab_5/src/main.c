#include "dijkstra.h"
#include "graph.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/* Печатает формат запуска программы */
static void print_usage(const char *program_name)
{
    printf("Usage: %s <input_file> <start_vertex> <graph_output_file> <result_output_file>\n", program_name);
}

/* Читает строку произвольной длины из stdin */
static char *read_line(void)
{
    int ch;
    int length = 0;
    int capacity = 16;
    char *line = malloc(sizeof(char) * capacity);

    if (line == NULL)
    {
        return NULL;
    }

    while ((ch = getchar()) != '\n' && ch != EOF)
    {
        if (length + 1 >= capacity)
        {
            int new_capacity = capacity * 2;
            char *new_line = realloc(line, sizeof(char) * new_capacity);

            if (new_line == NULL)
            {
                free(line);
                return NULL;
            }

            line = new_line;
            capacity = new_capacity;
        }

        line[length] = (char)ch;
        length++;
    }

    if (length > 0 && line[length - 1] == '\r')
    {
        length--;
    }

    line[length] = '\0';
    return line;
}

/* Выводит приглашение и читает строку произвольной длины */
static char *read_prompted_line(const char *prompt)
{
    printf("%s", prompt);
    fflush(stdout);

    return read_line();
}

/* Преобразует строку в int с проверкой ошибок */
static int parse_int(const char *text, int *value)
{
    char *end;
    long parsed;

    if (text == NULL || value == NULL)
    {
        return 0;
    }

    errno = 0;
    parsed = strtol(text, &end, 10);

    if (text == end || *end != '\0' || errno != 0 || parsed < INT_MIN || parsed > INT_MAX)
    {
        return 0;
    }

    *value = (int)parsed;
    return 1;
}

/* Загружает граф, запускает Дейкстру и сохраняет граф с результатом */
static int run_program(
    const char *input_file,
    int start_vertex,
    const char *graph_output_file,
    const char *result_output_file)
{
    Graph *graph = graph_load_from_file(input_file);
    if (graph == NULL)
    {
        printf("Ошибка: не удалось загрузить граф из файла %s\n", input_file);
        return 1;
    }

    if (!graph_save_to_file(graph, graph_output_file))
    {
        printf("Ошибка: не удалось сохранить граф в файл %s\n", graph_output_file);
        graph_free(graph);
        return 1;
    }

    DijkstraResult *result = dijkstra_run(graph, start_vertex);
    if (result == NULL)
    {
        printf("Ошибка: не удалось запустить алгоритм Дейкстры\n");
        graph_free(graph);
        return 1;
    }

    if (!dijkstra_result_save_to_file(result, result_output_file))
    {
        printf("Ошибка: не удалось сохранить результат в файл %s\n", result_output_file);
        dijkstra_result_free(result);
        graph_free(graph);
        return 1;
    }

    printf("Граф сохранён в %s\n", graph_output_file);
    printf("Результат сохранён в %s\n", result_output_file);

    dijkstra_result_free(result);
    graph_free(graph);
    return 0;
}

/* Запускает программу в интерактивном режиме */
static int run_cli(void)
{
    int status;
    int start_vertex;
    char *input_file = read_prompted_line("Файл графа: ");
    char *start_vertex_text = read_prompted_line("Стартовая вершина: ");
    char *graph_output_file = read_prompted_line("Файл для выгрузки графа: ");
    char *result_output_file = read_prompted_line("Файл для выгрузки результата: ");

    if (input_file == NULL ||
        start_vertex_text == NULL ||
        graph_output_file == NULL ||
        result_output_file == NULL)
    {
        printf("Ошибка: не удалось прочитать ввод\n");
        free(input_file);
        free(start_vertex_text);
        free(graph_output_file);
        free(result_output_file);
        return 1;
    }

    if (!parse_int(start_vertex_text, &start_vertex))
    {
        printf("Ошибка: стартовая вершина должна быть целым числом\n");
        free(input_file);
        free(start_vertex_text);
        free(graph_output_file);
        free(result_output_file);
        return 1;
    }

    status = run_program(input_file, start_vertex, graph_output_file, result_output_file);

    free(input_file);
    free(start_vertex_text);
    free(graph_output_file);
    free(result_output_file);
    return status;
}

/* Выбирает режим запуска по аргументам командной строки */
int main(int argc, char **argv)
{
    int start_vertex;

    if (argc == 1)
    {
        return run_cli();
    }

    if (argc != 5)
    {
        print_usage(argv[0]);
        return 1;
    }

    if (!parse_int(argv[2], &start_vertex))
    {
        printf("Ошибка: стартовая вершина должна быть целым числом\n");
        return 1;
    }

    return run_program(argv[1], start_vertex, argv[3], argv[4]);
}
