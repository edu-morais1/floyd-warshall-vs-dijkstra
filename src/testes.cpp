// testes.cpp - testes dos algoritmos (rodar com: make testes)

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "contagem.hpp"
#include "dijkstra.hpp"
#include "floyd_warshall.hpp"
#include "grafo.hpp"

using namespace std;

int falhas = 0;

void confere(bool ok, string descricao) {
    if (!ok) {
        cout << "FALHOU: " << descricao << endl;
        falhas++;
    }
}

// Monta um grafo não direcionado com n vértices a partir de uma lista de arestas {u, v, peso}.
Grafo montarGrafo(int n, vector<vector<int>> arestas) {
    Grafo g;
    g.n = n;
    g.peso.assign(n * n, 0);
    g.arestas = arestas.size();
    for (int k = 0; k < (int)arestas.size(); k++) {
        int u = arestas[k][0];
        int v = arestas[k][1];
        int w = arestas[k][2];
        g.peso[u * n + v] = w;
        g.peso[v * n + u] = w;
    }
    return g;
}

// Os três métodos e as três versões com contador têm de dar a matriz esperada.
void testaGrafo(string nome, Grafo g, Matriz esperado) {
    Contagem c;
    confere(floydWarshall(g) == esperado, "Floyd-Warshall: " + nome);
    confere(dijkstraTodosOsPares(g) == esperado, "Dijkstra com heap: " + nome);
    confere(dijkstraVetorTodosOsPares(g) == esperado, "Dijkstra com vetor: " + nome);
    confere(floydWarshallContando(g, c) == esperado, "Floyd-Warshall com contador: " + nome);
    confere(dijkstraTodosOsParesContando(g, c) == esperado, "Dijkstra com heap e contador: " + nome);
    confere(dijkstraVetorTodosOsParesContando(g, c) == esperado, "Dijkstra com vetor e contador: " + nome);
}

int main() {
    // 1. Grafos pequenos com resposta conhecida
    const int I = INF;
    Grafo um = montarGrafo(1, {});
    Grafo tri = montarGrafo(3, {{0, 1, 10}, {0, 2, 3}, {2, 1, 4}});   // de 0 a 1 é mais barato passar por 2
    Grafo caminho = montarGrafo(4, {{0, 1, 2}, {1, 2, 3}, {2, 3, 4}});
    Grafo dois = montarGrafo(4, {{0, 1, 5}, {2, 3, 7}});                // não há caminho entre {0,1} e {2,3}

    testaGrafo("1 vértice", um, {0});
    testaGrafo("triângulo", tri, {0, 7, 3, 7, 0, 4, 3, 4, 0});
    testaGrafo("caminho", caminho, {0, 2, 5, 9, 2, 0, 3, 7, 5, 3, 0, 4, 9, 7, 4, 0});
    testaGrafo("dois componentes", dois, {0, 5, I, I, 5, 0, I, I, I, I, 0, 7, I, I, 7, 0});

    // Entrada 10: os dois métodos dão a mesma matriz, e a soma das distâncias é 442
    Grafo e10 = lerGrafo("dados/Entrada 10.txt");
    Matriz fw = floydWarshall(e10);
    confere(fw == dijkstraTodosOsPares(e10), "Entrada 10: Floyd-Warshall e Dijkstra iguais");
    long long soma = 0;
    for (int i = 0; i < (int)fw.size(); i++)
        soma += fw[i];
    confere(e10.n == 10 && e10.arestas == 28 && soma == 442, "Entrada 10: 10 vértices, 28 arestas e soma 442");

    // 2. Contagens que dá para calcular à mão
    Contagem c;
    floydWarshallContando(tri, c);
    confere(c.relaxamentos == 27 && c.descartes == 9 && c.atualizacoes == 2, "FW no triângulo: 27, 9 e 2");
    floydWarshallContando(caminho, c);
    confere(c.relaxamentos == 52 && c.descartes == 16 && c.atualizacoes == 6, "FW no caminho: 13 linhas x 4");
    floydWarshallContando(dois, c);
    confere(c.relaxamentos == 32 && c.atualizacoes == 0, "FW em dois componentes: 8 linhas x 4");

    dijkstraTodosOsParesContando(tri, c);
    confere(c.relaxamentos == 18, "Dijkstra no triângulo: V * A = 3 * 6 relaxamentos");
    confere(c.descartes == 3 + c.atualizacoes, "Dijkstra no triângulo: uma remoção para cada inserção");
    dijkstraTodosOsParesContando(caminho, c);
    confere(c.relaxamentos == 24 && c.comparacoesFila > 0, "Dijkstra no caminho: 4 * 6 relaxamentos");
    dijkstraTodosOsParesContando(montarGrafo(2, {{0, 1, 6}}), c);
    confere(c.comparacoesFila == 0, "Dijkstra com 2 vértices: a fila nunca tem 2 pares");
    dijkstraTodosOsParesContando(e10, c);
    confere(c.relaxamentos == 560, "Entrada 10: 10 * 56 relaxamentos");
    confere(c.descartes == 10 + c.atualizacoes, "Entrada 10: uma remoção para cada inserção");
    // valores obtidos com a fila de prioridade da biblioteca padrão do g++
    confere(c.comparacoesFila == 464 && c.atualizacoes == 127, "Entrada 10: 464 comparações na fila e 127 atualizações");

    // Dijkstra com vetor: a cada passo, a busca compara as distâncias dos vértices
    // abertos (n(n-1)/2 comparações por origem num grafo conexo) e testa se o
    // vértice escolhido tem distância INF (n testes por origem)
    dijkstraVetorTodosOsParesContando(tri, c);
    confere(c.relaxamentos == 18 && c.comparacoesFila == 9 && c.descartes == 9 && c.atualizacoes == 8,
            "Dijkstra com vetor no triângulo: 18, 9, 9 e 8");
    dijkstraVetorTodosOsParesContando(e10, c);
    confere(c.relaxamentos == 560 && c.comparacoesFila == 450 && c.descartes == 100,
            "Entrada 10 com vetor: 560 relaxamentos, 10 * 45 comparações na busca e 100 testes de INF");

    // 3. Grafos sorteados: os seis resultados têm de ser iguais
    srand(2026);
    for (int t = 0; t < 200; t++) {
        int n = 1 + rand() % 30;
        int chance = 20 + (t % 5) * 20;   // chance de cada aresta existir: 20% a 100%
        vector<vector<int>> arestas;
        for (int u = 0; u < n; u++)
            for (int v = u + 1; v < n; v++)
                if (rand() % 100 < chance)
                    arestas.push_back({u, v, 1 + rand() % 50});
        Grafo g = montarGrafo(n, arestas);
        testaGrafo("sorteado " + to_string(t), g, floydWarshall(g));
    }

    if (falhas == 0)
        cout << "todos os testes passaram" << endl;
    else
        cout << falhas << " testes falharam" << endl;
    return falhas > 0 ? 1 : 0;
}
