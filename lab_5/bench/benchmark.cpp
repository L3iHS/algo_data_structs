#include <benchmark/benchmark.h>

extern "C"
{
#include "../src/dijkstra.h"
#include "../src/graph.h"
}

/* Создаёт детерминированный ориентированный граф для замеров */
static Graph *create_benchmark_graph(int vertices_count, int extra_edges_per_vertex)
{
    Graph *graph = graph_create(vertices_count);

    if (graph == nullptr)
    {
        return nullptr;
    }

    for (int vertex = 0; vertex < vertices_count - 1; vertex++)
    {
        graph_add_edge(graph, vertex, vertex + 1, 1);
    }

    for (int vertex = 0; vertex < vertices_count; vertex++)
    {
        for (int offset = 2; offset <= extra_edges_per_vertex + 1; offset++)
        {
            int to = (vertex + offset) % vertices_count;
            int weight = (vertex + offset) % 17 + 1;

            graph_add_edge(graph, vertex, to, weight);
        }
    }

    return graph;
}

/* Измеряет время работы алгоритма Дейкстры на графах разного размера */
static void BM_Dijkstra(benchmark::State &state)
{
    int vertices_count = static_cast<int>(state.range(0));
    int extra_edges_per_vertex = static_cast<int>(state.range(1));
    Graph *graph = create_benchmark_graph(vertices_count, extra_edges_per_vertex);

    if (graph == nullptr)
    {
        state.SkipWithError("failed to create graph");
        return;
    }

    for (auto _ : state)
    {
        DijkstraResult *result = dijkstra_run(graph, 0);

        if (result == nullptr)
        {
            state.SkipWithError("dijkstra_run failed");
            break;
        }

        benchmark::DoNotOptimize(result->distances[vertices_count - 1]);
        dijkstra_result_free(result);
    }

    state.SetItemsProcessed(state.iterations() * vertices_count);
    graph_free(graph);
}

BENCHMARK(BM_Dijkstra)
    ->Args({50, 4})
    ->Args({100, 6})
    ->Args({250, 8})
    ->Args({500, 10})
    ->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();