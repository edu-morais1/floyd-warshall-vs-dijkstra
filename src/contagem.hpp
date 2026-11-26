// contagem.hpp - versões dos três métodos que contam as comparações

#pragma once

#include "grafo.hpp"

using namespace std;

struct Contagem {
    long long relaxamentos = 0;      // testes de relaxamento
    long long comparacoesFila = 0;   // comparações da fila de prioridade do Dijkstra (no heap ou na busca no vetor)
    long long descartes = 0;         // FW: testes d[i][k] == INF; heap: teste de par antigo; vetor: teste de INF
    long long atualizacoes = 0;      // relaxamentos que diminuíram uma distância
};

Matriz floydWarshallContando(const Grafo& g, Contagem& c);
Matriz dijkstraTodosOsParesContando(const Grafo& g, Contagem& c);
Matriz dijkstraVetorTodosOsParesContando(const Grafo& g, Contagem& c);
