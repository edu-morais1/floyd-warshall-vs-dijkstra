// floyd_warshall.cpp - algoritmo de Floyd-Warshall (Aula 15, slides 41 e 42)
// Complexidade: O(V^3), não depende do número de arestas.
// Se mudar este código, mude também a cópia com contadores em contagem.cpp.

#include "floyd_warshall.hpp"

using namespace std;

Matriz floydWarshall(const Grafo& g) {
    int n = g.n;

    // matriz inicial: 0 na diagonal, o peso onde há aresta e INF onde não há
    Matriz d(n * n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j)
                d[i * n + j] = 0;
            else if (g.peso[i * n + j] > 0)
                d[i * n + j] = g.peso[i * n + j];
            else
                d[i * n + j] = INF;
        }
    }

    // k é o vértice intermediário liberado em cada rodada; a matriz é atualizada no próprio lugar
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            int dik = d[i * n + k];
            if (dik == INF)   // não há caminho de i até k, então nada melhora passando por k
                continue;
            for (int j = 0; j < n; j++) {
                int via = dik + d[k * n + j];
                if (via < d[i * n + j])
                    d[i * n + j] = via;
            }
        }
    }
    return d;
}
