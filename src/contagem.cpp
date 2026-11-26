// contagem.cpp - cópias do Floyd-Warshall e do Dijkstra com contadores
// As funções que têm o tempo medido não têm contadores, para a contagem não mudar
// o tempo. Estas cópias fazem as mesmas operações e são executadas à parte.

#include "contagem.hpp"

#include <queue>

#include "dijkstra.hpp"

using namespace std;

Matriz floydWarshallContando(const Grafo& g, Contagem& c) {
    c = Contagem();
    int n = g.n;

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

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            int dik = d[i * n + k];
            c.descartes++;
            if (dik == INF)
                continue;
            for (int j = 0; j < n; j++) {
                int via = dik + d[k * n + j];
                c.relaxamentos++;
                if (via < d[i * n + j]) {
                    d[i * n + j] = via;
                    c.atualizacoes++;
                }
            }
        }
    }
    return d;
}

// Comparações feitas pela fila de prioridade do Dijkstra.
static long long comparacoesNaFila = 0;

// Comparador da fila que conta quantas vezes é chamado. Usa a mesma ordem do
// greater<pair<int, int>> do Dijkstra medido, então a fila faz as mesmas operações.
struct ComparadorContando {
    bool operator()(const pair<int, int>& a, const pair<int, int>& b) const {
        comparacoesNaFila++;
        return a > b;
    }
};

static vector<int> dijkstraContando(const ListaAdj& adj, int origem, Contagem& c) {
    int n = adj.size();
    vector<int> dist(n, INF);
    dist[origem] = 0;

    priority_queue<pair<int, int>, vector<pair<int, int>>, ComparadorContando> fila;
    fila.push(make_pair(0, origem));

    while (!fila.empty()) {
        int du = fila.top().first;
        int u = fila.top().second;
        fila.pop();
        c.descartes++;
        if (du > dist[u])
            continue;

        for (const Aresta& arco : adj[u]) {
            int v = arco.destino;
            int nova = du + arco.peso;
            c.relaxamentos++;
            if (nova < dist[v]) {
                dist[v] = nova;
                fila.push(make_pair(nova, v));
                c.atualizacoes++;
            }
        }
    }
    return dist;
}

Matriz dijkstraTodosOsParesContando(const Grafo& g, Contagem& c) {
    c = Contagem();
    comparacoesNaFila = 0;

    int n = g.n;
    ListaAdj adj = montarListaAdj(g);
    Matriz d(n * n);
    for (int origem = 0; origem < n; origem++) {
        vector<int> dist = dijkstraContando(adj, origem, c);
        for (int v = 0; v < n; v++)
            d[origem * n + v] = dist[v];
    }

    c.comparacoesHeap = comparacoesNaFila;
    return d;
}
