// dijkstra.cpp - Dijkstra executado V vezes (Aula 15, slides 35 e 38)
// Cada execução calcula as distâncias a partir de uma origem; rodando uma vez para
// cada vértice, temos a matriz completa. Complexidade: O(V * A * log V) no total.
// Se mudar este código, mude também a cópia com contadores em contagem.cpp.

#include "dijkstra.hpp"

#include <functional>
#include <queue>

using namespace std;

// Monta a lista de adjacência a partir da matriz de pesos.
ListaAdj montarListaAdj(const Grafo& g) {
    int n = g.n;
    ListaAdj adj(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (g.peso[i * n + j] > 0) {
                Aresta a = {j, g.peso[i * n + j]};
                adj[i].push_back(a);
            }
        }
    }
    return adj;
}

vector<int> dijkstra(const ListaAdj& adj, int origem) {
    int n = adj.size();
    vector<int> dist(n, INF);
    dist[origem] = 0;

    // fila de prioridade com o par (distância, vértice) de menor distância no topo
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> fila;
    fila.push(make_pair(0, origem));

    while (!fila.empty()) {
        int du = fila.top().first;
        int u = fila.top().second;
        fila.pop();
        if (du > dist[u])   // par antigo: u já saiu da fila com uma distância menor
            continue;

        // relaxa os arcos que saem de u
        for (const Aresta& arco : adj[u]) {
            int v = arco.destino;
            int nova = du + arco.peso;
            if (nova < dist[v]) {
                dist[v] = nova;
                fila.push(make_pair(nova, v));   // no lugar do DECREASE-KEY
            }
        }
    }
    return dist;
}

Matriz dijkstraTodosOsPares(const Grafo& g) {
    int n = g.n;
    ListaAdj adj = montarListaAdj(g);
    Matriz d(n * n);
    for (int origem = 0; origem < n; origem++) {
        vector<int> dist = dijkstra(adj, origem);
        for (int v = 0; v < n; v++)
            d[origem * n + v] = dist[v];
    }
    return d;
}
