// grafo.hpp - estrutura do grafo e leitura dos arquivos de entrada

#pragma once

#include <string>
#include <vector>

using namespace std;

// "Infinito": distância até um vértice que não pode ser alcançado
const int INF = 1000000000;

// Matriz n x n guardada num vetor, linha por linha: o elemento (i, j) fica na posição i * n + j
typedef vector<int> Matriz;

struct Grafo {
    int n;              // número de vértices
    Matriz peso;        // matriz de pesos (0 = sem aresta)
    int arestas;        // número de arestas
};

Grafo lerGrafo(string caminho);
