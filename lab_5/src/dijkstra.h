#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"

#define INF_DISTANCE 1000000000

typedef struct
{
    int vertices_count; // сколько вершин в графе
    int start_vertex;   // от какой вершины считали кратчайшие пути
    int *distances;     // кратчайшее расстояние от start_vertex до вершины i
} DijkstraResult;

DijkstraResult *dijkstra_run(const Graph *graph, int start_vertex);
void dijkstra_result_free(DijkstraResult *result);

int dijkstra_result_save_to_file(const DijkstraResult *result, const char *filename);

#endif
