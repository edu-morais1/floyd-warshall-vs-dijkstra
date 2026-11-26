// dijkstra.hpp

#pragma once

#include <vector>

#include "grafo.hpp"

using namespace std;

// Um arco da lista de adjacência: vai até 'destino' com custo 'peso'.
struct Aresta {
    int destino;
    int peso;
};

typedef vector<vector<Aresta>> ListaAdj;

ListaAdj montarListaAdj(const Grafo& g);
vector<int> dijkstra(const ListaAdj& adj, int origem);
Matriz dijkstraTodosOsPares(const Grafo& g);
