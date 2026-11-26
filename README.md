# 2º Trabalho de PAA — Grupo 8

Comparação entre o algoritmo de Floyd-Warshall e o algoritmo de Dijkstra executado
V vezes (caminhos mínimos entre todos os pares de vértices), nos 15 grafos fornecidos
pelo professor. O programa mede o tempo dos dois métodos, confere se eles chegam às
mesmas distâncias e conta as comparações que cada um faz.

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

## Arquivos

| Arquivo | Conteúdo |
|---|---|
| `src/grafo.cpp` | leitura dos arquivos de entrada |
| `src/floyd_warshall.cpp` | Floyd-Warshall |
| `src/dijkstra.cpp` | Dijkstra e Dijkstra executado V vezes |
| `src/contagem.cpp` | cópias dos dois algoritmos que contam as comparações |
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
- São 10 medidas de cada método em cada grafo, trocando a ordem dos métodos a cada
  medida.
- O comando de medição está em `src/experimento.cpp`, na linha marcada com
  `<- a medição`.

## Como as comparações são contadas

As funções medidas não têm contadores. A contagem usa cópias delas com contadores,
em `src/contagem.cpp`, executadas uma vez por grafo, fora da medição de tempo (linha
marcada com `<- a contagem` em `src/experimento.cpp`). São contadas:

- **relaxamentos:** o teste `d[i][k] + d[k][j] < d[i][j]` no Floyd-Warshall e
  `dist[u] + w < dist[v]` no Dijkstra;
- **comparações do heap:** as comparações feitas pela fila de prioridade do Dijkstra;
- **descartes:** o teste que pula a linha sem caminho no Floyd-Warshall e o teste de
  par antigo a cada remoção da fila no Dijkstra;
- **atualizações:** os relaxamentos que diminuíram uma distância.

## Saídas

| Arquivo | Colunas |
|---|---|
| `resultados/tempos.csv` | `n,algoritmo,repeticao,execucoes,tempo_s` (tempo de uma execução) |
| `resultados/validacao.csv` | `n,arestas,soma_fw,soma_dijkstra,maior_distancia,diferencas` |
| `resultados/contagens.csv` | `n,arestas,algoritmo,comparacoes,relaxamentos,comparacoes_heap,descartes,atualizacoes` |
| `resultados/resumo.csv` | `n,algoritmo,media_s,desvio_s,cv` |

Na entrega, o código e o relatório em PDF vão compactados num arquivo só, sem a
pasta `build/`.
