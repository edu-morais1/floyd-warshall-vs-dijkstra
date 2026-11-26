// experimento.cpp - mede o tempo do Floyd-Warshall e do Dijkstra V vezes (com heap
// e com vetor) nos 15 grafos, confere se as distâncias são iguais e conta as
// comparações.
// Rodar com: make rodar (os resultados vão para a pasta resultados/)

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

#include "contagem.hpp"
#include "dijkstra.hpp"
#include "floyd_warshall.hpp"
#include "grafo.hpp"

using namespace std;

const int GRAFOS = 15;             // número de grafos de entrada
const int REPETICOES = 10;         // medidas de cada método em cada grafo
const double TEMPO_MINIMO = 0.1;   // cada medida dura pelo menos 0,1 segundo

const int METODOS = 3;
const string NOMES[METODOS] = {"floyd_warshall", "dijkstra_v_vezes", "dijkstra_vetor"};

// Executa o método escolhido: 0 = Floyd-Warshall, 1 = Dijkstra V vezes com heap,
// 2 = Dijkstra V vezes com vetor.
Matriz executar(int metodo, const Grafo& g) {
    if (metodo == 0)
        return floydWarshall(g);
    else if (metodo == 1)
        return dijkstraTodosOsPares(g);
    else
        return dijkstraVetorTodosOsPares(g);
}

// Mede o tempo de uma execução do método. Nos grafos pequenos uma execução leva
// poucos microssegundos, então o método é repetido até completar TEMPO_MINIMO e o
// tempo total é dividido pelo número de execuções.
double medir(int metodo, const Grafo& g, Matriz& resultado, int& execucoes) {
    chrono::steady_clock::time_point inicio = chrono::steady_clock::now();
    double passado = 0;
    execucoes = 0;
    while (passado < TEMPO_MINIMO) {
        resultado = executar(metodo, g);   // <- a medição
        execucoes++;
        passado = chrono::duration<double>(chrono::steady_clock::now() - inicio).count();
    }
    return passado / execucoes;
}

int main() {
    int tamanhos[GRAFOS] = {10, 25, 50, 75, 100, 150, 200, 250, 300, 400, 500, 650, 800, 1000, 1500};

    ofstream tempos("resultados/tempos.csv");
    ofstream validacao("resultados/validacao.csv");
    ofstream contagens("resultados/contagens.csv");
    if (!tempos || !validacao || !contagens) {
        cout << "Erro: não foi possível criar os arquivos em resultados/" << endl;
        return 1;
    }
    tempos << "n,algoritmo,repeticao,execucoes,tempo_s\n";
    validacao << "n,arestas,soma_distancias,maior_distancia,diferencas\n";
    contagens << "n,arestas,algoritmo,comparacoes,relaxamentos,comparacoes_fila,descartes,atualizacoes\n";

    cout << "n\tFW (s)\t\theap (s)\tvetor (s)\theap/FW\tvetor/FW\tdistâncias" << endl;
    bool tudoIgual = true;

    for (int t = 0; t < GRAFOS; t++) {
        Grafo g = lerGrafo("dados/Entrada " + to_string(tamanhos[t]) + ".txt");
        int n = g.n;   // o n do próprio arquivo, que é o tamanho real das matrizes

        // aquecimento: uma execução de cada método antes de medir
        for (int m = 0; m < METODOS; m++)
            executar(m, g);

        // medição: a ordem dos métodos gira a cada repetição (0 1 2, 1 2 0, 2 0 1, ...),
        // para que uma variação na velocidade da máquina não prejudique sempre o mesmo método
        Matriz resultado[METODOS];
        double soma[METODOS] = {0, 0, 0};
        for (int r = 0; r < REPETICOES; r++) {
            for (int i = 0; i < METODOS; i++) {
                int metodo = (i + r) % METODOS;
                int execucoes;
                double tempo = medir(metodo, g, resultado[metodo], execucoes);
                tempos << n << "," << NOMES[metodo] << "," << r << "," << execucoes << "," << tempo << "\n";
                soma[metodo] += tempo;
            }
        }

        // as três matrizes de distâncias têm de ser iguais, entrada por entrada
        long long diferencas = 0, soma_distancias = 0;
        int maior = 0;
        for (int i = 0; i < n * n; i++) {
            if (resultado[1][i] != resultado[0][i] || resultado[2][i] != resultado[0][i])
                diferencas++;
            soma_distancias += resultado[0][i];
            if (resultado[0][i] > maior)
                maior = resultado[0][i];
        }
        validacao << n << "," << g.arestas << "," << soma_distancias << "," << maior << "," << diferencas << "\n";
        if (diferencas > 0)
            tudoIgual = false;

        // contagem das comparações, numa execução à parte (fora da medição de tempo)
        Contagem c[METODOS];
        floydWarshallContando(g, c[0]);               // <- a contagem
        dijkstraTodosOsParesContando(g, c[1]);        // <- a contagem
        dijkstraVetorTodosOsParesContando(g, c[2]);   // <- a contagem
        for (int m = 0; m < METODOS; m++) {
            long long total = c[m].relaxamentos + c[m].comparacoesFila + c[m].descartes;
            contagens << n << "," << g.arestas << "," << NOMES[m] << "," << total << "," << c[m].relaxamentos
                      << "," << c[m].comparacoesFila << "," << c[m].descartes << "," << c[m].atualizacoes << "\n";
        }

        // grava agora os resultados deste grafo (se o programa for interrompido, eles ficam salvos)
        tempos.flush();
        validacao.flush();
        contagens.flush();

        double mediaFw = soma[0] / REPETICOES;
        double mediaHeap = soma[1] / REPETICOES;
        double mediaVetor = soma[2] / REPETICOES;
        cout << n << "\t" << mediaFw << "\t" << mediaHeap << "\t" << mediaVetor << "\t" << mediaHeap / mediaFw
             << "\t" << mediaVetor / mediaFw << "\t" << (diferencas == 0 ? "iguais" : "DIFERENTES") << endl;
    }

    return tudoIgual ? 0 : 1;
}
