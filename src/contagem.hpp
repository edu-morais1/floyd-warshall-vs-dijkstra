// contagem.hpp - versões dos dois algoritmos que contam as comparações

#pragma once

#include "grafo.hpp"

using namespace std;

struct Contagem {
    long long relaxamentos = 0;      // testes de relaxamento
    long long comparacoesHeap = 0;   // comparações feitas dentro da fila de prioridade (só no Dijkstra)
    long long descartes = 0;         // FW: testes d[i][k] == INF; Dijkstra: teste de par antigo a cada remoção
    long long atualizacoes = 0;      // relaxamentos que diminuíram uma distância
};

Matriz floydWarshallContando(const Grafo& g, Contagem& c);
Matriz dijkstraTodosOsParesContando(const Grafo& g, Contagem& c);
