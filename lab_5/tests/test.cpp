#include <gtest/gtest.h>

extern "C"
{
#include "../src/dijkstra.h"
#include "../src/graph.h"
}

/* Проверяет создание пустого графа с корректным количеством вершин */
TEST(GraphTest, CreatesEmptyGraph)
{
    Graph *graph = graph_create(5);

    ASSERT_NE(graph, nullptr);
    EXPECT_EQ(graph->vertices_count, 5);
    EXPECT_EQ(graph->edges_count, 0);
    EXPECT_GE(graph->edges_capacity, 1);
    EXPECT_NE(graph->edges, nullptr);

    graph_free(graph);
}

/* Проверяет добавление корректного ребра в граф */
TEST(GraphTest, AddsValidEdge)
{
    Graph *graph = graph_create(3);

    ASSERT_NE(graph, nullptr);
    EXPECT_EQ(graph_add_edge(graph, 0, 1, 10), 1);

    ASSERT_EQ(graph->edges_count, 1);
    EXPECT_EQ(graph->edges[0].from, 0);
    EXPECT_EQ(graph->edges[0].to, 1);
    EXPECT_EQ(graph->edges[0].weight, 10);

    graph_free(graph);
}

/* Проверяет, что граф отклоняет некорректные рёбра */
TEST(GraphTest, RejectsInvalidEdges)
{
    Graph *graph = graph_create(3);

    ASSERT_NE(graph, nullptr);
    EXPECT_EQ(graph_add_edge(graph, -1, 1, 10), 0);
    EXPECT_EQ(graph_add_edge(graph, 0, 3, 10), 0);
    EXPECT_EQ(graph_add_edge(graph, 0, 1, -5), 0);
    EXPECT_EQ(graph->edges_count, 0);

    graph_free(graph);
}

/* Проверяет, что Дейкстра выбирает более короткий путь через промежуточную вершину */
TEST(DijkstraTest, FindsShortestPathThroughIntermediateVertex)
{
    Graph *graph = graph_create(3);

    ASSERT_NE(graph, nullptr);
    ASSERT_EQ(graph_add_edge(graph, 0, 1, 10), 1);
    ASSERT_EQ(graph_add_edge(graph, 0, 2, 3), 1);
    ASSERT_EQ(graph_add_edge(graph, 2, 1, 4), 1);

    DijkstraResult *result = dijkstra_run(graph, 0);

    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->distances[0], 0);
    EXPECT_EQ(result->distances[1], 7);
    EXPECT_EQ(result->distances[2], 3);

    dijkstra_result_free(result);
    graph_free(graph);
}

/* Проверяет, что недостижимые вершины остаются с расстоянием INF_DISTANCE */
TEST(DijkstraTest, KeepsUnreachableVerticesAsInfinity)
{
    Graph *graph = graph_create(4);

    ASSERT_NE(graph, nullptr);
    ASSERT_EQ(graph_add_edge(graph, 0, 1, 5), 1);

    DijkstraResult *result = dijkstra_run(graph, 0);

    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->distances[0], 0);
    EXPECT_EQ(result->distances[1], 5);
    EXPECT_EQ(result->distances[2], INF_DISTANCE);
    EXPECT_EQ(result->distances[3], INF_DISTANCE);

    dijkstra_result_free(result);
    graph_free(graph);
}

/* Проверяет, что Дейкстра отклоняет стартовую вершину вне диапазона */
TEST(DijkstraTest, RejectsInvalidStartVertex)
{
    Graph *graph = graph_create(3);

    ASSERT_NE(graph, nullptr);
    EXPECT_EQ(dijkstra_run(graph, -1), nullptr);
    EXPECT_EQ(dijkstra_run(graph, 3), nullptr);

    graph_free(graph);
}
