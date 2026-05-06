#include "dijkstra.h"
#include <stdio.h>
#include <stdlib.h>

/* Находит кратчайшие расстояния от стартовой вершины до всех остальных */
DijkstraResult *dijkstra_run(const Graph *graph, int start_vertex)
{
    if (graph == NULL || start_vertex < 0 || start_vertex >= graph->vertices_count)
    {
        return NULL;
    }

    DijkstraResult *result = malloc(sizeof(DijkstraResult));
    if (result == NULL)
    {
        return NULL;
    }

    result->distances = malloc(sizeof(int) * graph->vertices_count);
    if (result->distances == NULL)
    {
        free(result);
        return NULL;
    }

    int *visited = malloc(sizeof(int) * graph->vertices_count);
    if (visited == NULL)
    {
        free(result->distances);
        free(result);
        return NULL;
    }

    result->vertices_count = graph->vertices_count;
    result->start_vertex = start_vertex;

    // Изначально расстояния до всех вершин неизвестны
    for (int i = 0; i < graph->vertices_count; i++)
    {
        result->distances[i] = INF_DISTANCE;
    }

    // В начале ни одна вершина не обработана окончательно
    for (int i = 0; i < graph->vertices_count; i++)
    {
        visited[i] = 0;
    }

    result->distances[start_vertex] = 0;

    // Алгоритм Дейкстры
    for (int step = 0; step < graph->vertices_count; step++)
    {
        int current = -1;
        for (int i = 0; i < graph->vertices_count; i++)
        {
            if (!visited[i] && (current == -1 || result->distances[i] < result->distances[current]))
            {
                current = i;
            }
        }

        if (current == -1 || result->distances[current] == INF_DISTANCE)
        {
            break;
        }

        visited[current] = 1;

        for (int e = 0; e < graph->edges_count; e++)
        {
            Edge edge = graph->edges[e];
            if (edge.from == current)
            {
                int new_distance = result->distances[current] + edge.weight;

                // Релаксация ребра: сохраняем путь, если он оказался короче
                if (new_distance < result->distances[edge.to])
                {
                    result->distances[edge.to] = new_distance;
                }
            }
        }
    }

    free(visited);
    return result;
}

/* Освобождает результат работы алгоритма Дейкстры */
void dijkstra_result_free(DijkstraResult *result)
{
    if (result != NULL)
    {
        free(result->distances);
        free(result);
    }
}

/* Сохраняет кратчайшие расстояния в текстовый файл */
int dijkstra_result_save_to_file(const DijkstraResult *result, const char *filename)
{
    if (result == NULL || filename == NULL)
    {
        return 0;
    }

    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        return 0;
    }

    fprintf(file, "start %d\n", result->start_vertex);

    for (int i = 0; i < result->vertices_count; i++)
    {
        if (result->distances[i] == INF_DISTANCE)
        {
            fprintf(file, "%d INF\n", i);
        }
        else
        {
            fprintf(file, "%d %d\n", i, result->distances[i]);
        }
    }

    fclose(file);
    return 1;
}
