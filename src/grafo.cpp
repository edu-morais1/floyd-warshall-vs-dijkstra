// grafo.cpp - leitura dos arquivos "Entrada N.txt"

#include "grafo.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>

using namespace std;

// O arquivo tem o número de vértices na primeira linha e depois a matriz de pesos.
Grafo lerGrafo(string caminho) {
    ifstream arquivo(caminho);
    if (!arquivo) {
        cout << "Erro: não foi possível abrir " << caminho << endl;
        exit(1);
    }

    Grafo g;
    arquivo >> g.n;
    g.peso.resize(g.n * g.n);
    for (int i = 0; i < g.n * g.n; i++)
        arquivo >> g.peso[i];
    if (!arquivo) {
        cout << "Erro: faltam números em " << caminho << endl;
        exit(1);
    }

    // cada aresta aparece duas vezes na matriz, então só conta a parte acima da diagonal
    g.arestas = 0;
    for (int i = 0; i < g.n; i++)
        for (int j = i + 1; j < g.n; j++)
            if (g.peso[i * g.n + j] > 0)
                g.arestas++;
    return g;
}
