#ifndef GRAPH_H
#define GRAPH_H

typedef struct
{
    int from;   // из какой вершины идёт ребро
    int to;     // в какую вершину идёт ребро
    int weight; // вес ребра
} Edge;

typedef struct
{
    int vertices_count; // сколько вершин в графе
    int edges_count;    // сколько рёбер реально добавлено
    int edges_capacity; // сколько рёбер сейчас помещается в выделенный массив
    Edge *edges;        // динамический массив рёбер
} Graph;

Graph *graph_create(int vertices_count);
void graph_free(Graph *graph);

int graph_add_edge(Graph *graph, int from, int to, int weight);

#endif