#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

/* Создаёт пустой граф с заданным количеством вершин */
Graph *graph_create(int vertices_count)
{
    if (vertices_count <= 0)
    {
        return NULL;
    }

    Graph *graph = malloc(sizeof(Graph));

    if (graph == NULL)
    {
        return NULL;
    }

    graph->vertices_count = vertices_count;
    graph->edges_count = 0;
    graph->edges_capacity = 8;
    graph->edges = malloc(sizeof(Edge) * graph->edges_capacity);

    if (graph->edges == NULL)
    {
        free(graph);
        return NULL;
    }

    return graph;
}

/* Освобождает память, выделенную под граф */
void graph_free(Graph *graph)
{
    if (graph != NULL)
    {
        free(graph->edges);
        free(graph);
    }
}

/* Добавляет ориентированное ребро с неотрицательным весом */
int graph_add_edge(Graph *graph, int from, int to, int weight)
{
    if (graph == NULL ||
        from < 0 ||
        to < 0 ||
        from >= graph->vertices_count ||
        to >= graph->vertices_count ||
        weight < 0) // допустимый диапазон
    {
        return 0;
    }

    if (graph->edges_count == graph->edges_capacity)
    {
        int new_capacity = graph->edges_capacity * 2;
        Edge *new_edges = realloc(graph->edges, sizeof(Edge) * new_capacity);

        if (new_edges == NULL)
        {
            return 0;
        }

        graph->edges = new_edges;
        graph->edges_capacity = new_capacity;
    }

    graph->edges[graph->edges_count].from = from;
    graph->edges[graph->edges_count].to = to;
    graph->edges[graph->edges_count].weight = weight;

    graph->edges_count++;
    return 1;
}

/* Сохраняет граф в текстовый файл */
int graph_save_to_file(const Graph *graph, const char *filename)
{
    if (graph == NULL || filename == NULL)
    {
        return 0;
    }

    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        return 0;
    }

    fprintf(file, "%d %d\n", graph->vertices_count, graph->edges_count);

    for (int i = 0; i < graph->edges_count; i++)
    {
        fprintf(file, "%d %d %d\n",
                graph->edges[i].from,
                graph->edges[i].to,
                graph->edges[i].weight);
    }

    fclose(file);
    return 1;
}

/* Загружает граф из текстового файла */
Graph *graph_load_from_file(const char *filename)
{
    if (filename == NULL)
    {
        return NULL;
    }

    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        return NULL;
    }

    int vertices_count, edges_count;
    if (fscanf(file, "%d %d", &vertices_count, &edges_count) != 2 || edges_count < 0)
    {
        fclose(file);
        return NULL;
    }

    Graph *graph = graph_create(vertices_count);

    if (graph == NULL)
    {
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < edges_count; i++)
    {
        int from, to, weight;
        if (fscanf(file, "%d %d %d", &from, &to, &weight) != 3)
        {
            graph_free(graph);
            fclose(file);
            return NULL;
        }

        if (!graph_add_edge(graph, from, to, weight))
        {
            graph_free(graph);
            fclose(file);
            return NULL;
        }
    }

    fclose(file);
    return graph;
}
