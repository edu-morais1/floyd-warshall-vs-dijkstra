# 2º Trabalho de PAA — Grupo 8

Comparação entre o algoritmo de Floyd-Warshall e o algoritmo de Dijkstra executado
V vezes (caminhos mínimos entre todos os pares de vértices), nos 15 grafos fornecidos
pelo professor. O Dijkstra foi implementado com duas filas de prioridade: heap
binário e vetor (busca linear do mínimo). O programa mede o tempo dos três métodos,
confere se eles chegam às mesmas distâncias e conta as comparações que cada um faz.

## Dados de entrada

Os 15 grafos de entrada foram fornecidos pela disciplina e não estão neste
repositório. Antes de rodar, coloque os arquivos `Entrada 10.txt`, `Entrada 25.txt`,
..., `Entrada 1500.txt` numa pasta `dados/`, dentro desta pasta. Cada arquivo tem o
número de vértices na primeira linha e depois a matriz de pesos (0 = sem aresta).

## Como compilar e rodar

É preciso ter `g++` (C++17) e `make`. Para as figuras, Python 3 com `matplotlib`.

```
make testes      # compila e roda os testes
make rodar       # roda o experimento (uns 8 minutos) e grava os CSVs em resultados/
make graficos    # gera as figuras em resultados/figuras/
```

## No Windows

O código em C++ e o script das figuras funcionam no Windows, mas o Makefile usa
comandos do Linux. Há três caminhos:

1. **WSL** (o Linux que roda dentro do Windows): os comandos acima funcionam sem
   mudança.
2. **MSYS2:** instale o `g++` e o `make` (`pacman -S mingw-w64-ucrt-x86_64-gcc make`)
   e use os comandos acima no terminal do MSYS2.
3. **Sem o `make`**, no PowerShell, de dentro desta pasta:

   ```
   mkdir build, resultados
   g++ -std=c++17 -O2 -Wall src/testes.cpp src/grafo.cpp src/floyd_warshall.cpp src/dijkstra.cpp src/contagem.cpp -o build/testes.exe
   .\build\testes.exe
   g++ -std=c++17 -O2 -Wall src/experimento.cpp src/grafo.cpp src/floyd_warshall.cpp src/dijkstra.cpp src/contagem.cpp -o build/experimento.exe
   .\build\experimento.exe
   pip install matplotlib
   python scripts/graficos.py
   ```

   A pasta `resultados` precisa existir antes de rodar o experimento; o `mkdir`
   acima já a cria.

Observações:

- No `cmd` antigo, os acentos do terminal podem sair trocados; use o Windows
  Terminal ou rode `chcp 65001` antes.
- Com o compilador do Visual Studio, o teste que confere o número exato de
  comparações do heap na Entrada 10 pode falhar, porque esse número depende da
  implementação da fila de prioridade. As distâncias e os relaxamentos não mudam.
- Para medições mais estáveis, feche os outros programas e use o plano de energia
  "Alto desempenho".

## Arquivos

| Arquivo | Conteúdo |
|---|---|
| `src/grafo.cpp` | leitura dos arquivos de entrada |
| `src/floyd_warshall.cpp` | Floyd-Warshall |
| `src/dijkstra.cpp` | Dijkstra executado V vezes, com heap e com vetor |
| `src/contagem.cpp` | cópias dos três métodos que contam as comparações |
| `src/experimento.cpp` | mede os tempos, compara as distâncias e conta as comparações |
| `src/testes.cpp` | testes dos algoritmos |
| `scripts/graficos.py` | figuras e `resumo.csv` |
| `dados/` | os 15 grafos (não incluídos; veja "Dados de entrada") |
| `resultados/` | CSVs e figuras gerados |

O relatório do trabalho não está neste repositório por enquanto.

## Como o tempo é medido

- O tempo vai do grafo já carregado na memória até a matriz de distâncias pronta;
  a leitura do arquivo fica de fora.
- Antes de medir, cada método roda uma vez para aquecer a máquina.
- Cada medida repete o método até completar 0,1 s e divide o tempo pelo número de
  execuções, porque nos grafos pequenos uma execução leva poucos microssegundos.
- São 10 medidas de cada método em cada grafo, girando a ordem dos três métodos a
  cada medida.
- O comando de medição está em `src/experimento.cpp`, na linha marcada com
  `<- a medição`.

## Como as comparações são contadas

As funções medidas não têm contadores. A contagem usa cópias delas com contadores,
em `src/contagem.cpp`, executadas uma vez por grafo, fora da medição de tempo (linha
marcada com `<- a contagem` em `src/experimento.cpp`). São contadas:

- **relaxamentos:** o teste `d[i][k] + d[k][j] < d[i][j]` no Floyd-Warshall e
  `dist[u] + w < dist[v]` no Dijkstra;
- **comparações da fila:** no Dijkstra com heap, as comparações feitas pela fila de
  prioridade; no Dijkstra com vetor, as comparações da busca do vértice mais próximo;
- **descartes:** o teste que pula a linha sem caminho no Floyd-Warshall, o teste de
  par antigo a cada remoção da fila no Dijkstra com heap e o teste de distância
  infinita a cada passo no Dijkstra com vetor;
- **atualizações:** os relaxamentos que diminuíram uma distância.

## Saídas

| Arquivo | Colunas |
|---|---|
| `resultados/tempos.csv` | `n,algoritmo,repeticao,execucoes,tempo_s` (tempo de uma execução) |
| `resultados/validacao.csv` | `n,arestas,soma_distancias,maior_distancia,diferencas` (`diferencas` conta as entradas em que algum Dijkstra difere do Floyd-Warshall) |
| `resultados/contagens.csv` | `n,arestas,algoritmo,comparacoes,relaxamentos,comparacoes_fila,descartes,atualizacoes` |
| `resultados/resumo.csv` | `n,algoritmo,media_s,desvio_s,cv` |

Na coluna `algoritmo`, `floyd_warshall` é o Floyd-Warshall, `dijkstra_v_vezes` é o
Dijkstra com heap e `dijkstra_vetor` é o Dijkstra com vetor.

Na entrega, o código e o relatório em PDF vão compactados num arquivo só, sem a
pasta `build/`.
