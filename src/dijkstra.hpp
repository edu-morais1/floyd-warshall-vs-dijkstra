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

// Dijkstra com a fila de prioridade feita com heap binário
vector<int> dijkstra(const ListaAdj& adj, int origem);
Matriz dijkstraTodosOsPares(const Grafo& g);

// Dijkstra com a fila de prioridade feita com vetor (busca linear do mínimo)
vector<int> dijkstraVetor(const ListaAdj& adj, int origem);
Matriz dijkstraVetorTodosOsPares(const Grafo& g);
