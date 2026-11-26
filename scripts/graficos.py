# graficos.py - gera as figuras do relatório a partir dos CSVs da pasta resultados/
# Rodar de dentro da pasta codigo/: python3 scripts/graficos.py

import csv
import math
import os
import statistics

import matplotlib
matplotlib.use("Agg")   # só salva os arquivos, sem abrir janela
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter

PASTA = "resultados"
# caches do processador onde a rodada foi feita (Ryzen 7 5700X: L2 de 512 KiB por núcleo, L3 de 32 MiB)
CACHE_L2 = 512 * 1024
CACHE_L3 = 32 * 1024 * 1024
FIGURAS = os.path.join(PASTA, "figuras")
ALGORITMOS = ["floyd_warshall", "dijkstra_v_vezes", "dijkstra_vetor"]
NOMES = {"floyd_warshall": "Floyd-Warshall", "dijkstra_v_vezes": "Dijkstra com heap",
         "dijkstra_vetor": "Dijkstra com vetor"}
MARCADORES = {"floyd_warshall": "o", "dijkstra_v_vezes": "s", "dijkstra_vetor": "^"}
# cores que continuam distinguíveis para quem tem daltonismo; o marcador ajuda na impressão em preto e branco
CORES = {"floyd_warshall": "#2a78d6", "dijkstra_v_vezes": "#eb6834", "dijkstra_vetor": "#1baf7a"}
CINZA = "#6b6a66"
TEXTO = "#333333"
TAMANHOS_NO_EIXO = [10, 25, 50, 100, 250, 500, 1000]

# figuras com a largura do texto do relatório (15 cm), para as letras saírem no tamanho certo
plt.rcParams.update({
    "figure.figsize": (5.9, 2.9),
    "font.size": 10,
    "axes.spines.top": False,
    "axes.spines.right": False,
    "axes.edgecolor": CINZA,
    "axes.labelcolor": TEXTO,
    "xtick.color": CINZA,
    "ytick.color": CINZA,
    "xtick.labelcolor": TEXTO,
    "ytick.labelcolor": TEXTO,
    "lines.linewidth": 2,
    "lines.markersize": 5,
    "legend.frameon": False,
    "legend.fontsize": 9,
    "figure.constrained_layout.use": True,
})


def virgula(valor, posicao):
    # números dos eixos com vírgula decimal
    return f"{valor:g}".replace(".", ",")


def com_virgula(valor, casas):
    return f"{valor:.{casas}f}".replace(".", ",")


def eixo_x_log():
    plt.xscale("log")
    plt.xticks(TAMANHOS_NO_EIXO, [str(n) for n in TAMANHOS_NO_EIXO])
    plt.minorticks_off()
    plt.xlim(8, 2900)   # sobra espaço à direita para os rótulos do último ponto
    plt.xlabel("número de vértices (n)")


def legenda(colunas):
    # legenda numa linha acima do gráfico, sem cobrir as curvas
    plt.legend(loc="lower left", bbox_to_anchor=(0, 1.0), ncol=colunas, handlelength=2.5, columnspacing=1.5)


def rotulo(x, y, texto, deslocamento=0):
    # texto à direita de um ponto (deslocamento vertical em pontos tipográficos)
    plt.annotate(texto, (x, y), xytext=(7, deslocamento), textcoords="offset points",
                 va="center", fontsize=9, color=TEXTO)


def salvar(nome):
    plt.grid(True, axis="y", color="#dddddd", linewidth=0.8)
    plt.gca().set_axisbelow(True)
    plt.savefig(os.path.join(FIGURAS, nome + ".pdf"))
    plt.savefig(os.path.join(FIGURAS, nome + ".png"), dpi=150)
    plt.close()


# tempos[algoritmo][n] = lista com o tempo de cada repetição
tempos = {"floyd_warshall": {}, "dijkstra_v_vezes": {}, "dijkstra_vetor": {}}
with open(os.path.join(PASTA, "tempos.csv")) as arquivo:
    for linha in csv.DictReader(arquivo):
        n = int(linha["n"])
        alg = linha["algoritmo"]
        if n not in tempos[alg]:
            tempos[alg][n] = []
        tempos[alg][n].append(float(linha["tempo_s"]))

ns = sorted(tempos["floyd_warshall"])
media = {}
desvio = {}
for alg in ALGORITMOS:
    media[alg] = [statistics.mean(tempos[alg][n]) for n in ns]
    desvio[alg] = [statistics.stdev(tempos[alg][n]) for n in ns]

# resumo.csv: média, desvio-padrão e coeficiente de variação (desvio / média)
with open(os.path.join(PASTA, "resumo.csv"), "w") as arquivo:
    arquivo.write("n,algoritmo,media_s,desvio_s,cv\n")
    for alg in ALGORITMOS:
        for i in range(len(ns)):
            cv = desvio[alg][i] / media[alg][i]
            arquivo.write(f"{ns[i]},{alg},{media[alg][i]},{desvio[alg][i]},{cv}\n")

os.makedirs(FIGURAS, exist_ok=True)
ultimo = ns[-1]

# 1: tempo por execução em escala linear, com o desvio-padrão e o tempo do maior grafo
plt.figure()
for alg in ALGORITMOS:
    plt.errorbar(ns, media[alg], yerr=desvio[alg], marker=MARCADORES[alg], color=CORES[alg],
                 capsize=2, label=NOMES[alg])
    rotulo(ultimo, media[alg][-1], com_virgula(media[alg][-1], 2) + " s")
plt.xlim(0, 1750)
plt.xticks([0, 250, 500, 750, 1000, 1250, 1500])
plt.ylim(bottom=0)
plt.gca().yaxis.set_major_formatter(FuncFormatter(virgula))
plt.xlabel("número de vértices (n)")
plt.ylabel("tempo por execução (s)")
legenda(3)
salvar("tempo_linear")

# 2: tempo por execução em escala log-log, com as retas de referência n³ e n³·log n
# passando pelo ponto do Floyd-Warshall no maior grafo
plt.figure()
for alg in ALGORITMOS:
    plt.plot(ns, media[alg], marker=MARCADORES[alg], color=CORES[alg], label=NOMES[alg])
t = media["floyd_warshall"][-1]
plt.plot(ns, [t * (n / ultimo) ** 3 for n in ns], ":", color=CINZA, linewidth=1.2, label="referência n³")
plt.plot(ns, [t * n ** 3 * math.log(n) / (ultimo ** 3 * math.log(ultimo)) for n in ns], "--", color=CINZA,
         linewidth=1.2, label="referência n³·log n")
plt.yscale("log")
eixo_x_log()
plt.yticks([1e-6, 1e-5, 1e-4, 1e-3, 1e-2, 1e-1, 1],
           ["1 µs", "10 µs", "100 µs", "1 ms", "10 ms", "100 ms", "1 s"])
plt.minorticks_off()
plt.xlim(8, 1900)
plt.ylabel("tempo por execução")
plt.legend(loc="lower right")
salvar("tempo_loglog")

# 3: razão entre o tempo de cada Dijkstra e o do Floyd-Warshall
# (acima de 1, o Floyd-Warshall é mais rápido)
plt.figure()
for alg in ["dijkstra_v_vezes", "dijkstra_vetor"]:
    razao = [media[alg][i] / media["floyd_warshall"][i] for i in range(len(ns))]
    plt.plot(ns, razao, marker=MARCADORES[alg], color=CORES[alg], label=NOMES[alg])
    rotulo(ultimo, razao[-1], com_virgula(razao[-1], 2))
plt.axhline(1, color=CINZA, linewidth=1.2)
plt.text(9, 1.15, "1 = mesmo tempo do Floyd-Warshall", fontsize=9, color=TEXTO)
# onde o heap passa a ganhar do vetor
i = ns.index(500)
plt.annotate("a partir de n = 500, o heap\nfica mais rápido que o vetor", (500, media["dijkstra_v_vezes"][i] / media["floyd_warshall"][i]),
             xytext=(230, 4.6), fontsize=9, color=TEXTO, arrowprops={"arrowstyle": "->", "color": CINZA})
eixo_x_log()
plt.ylim(0, 8.3)
plt.gca().yaxis.set_major_formatter(FuncFormatter(virgula))
plt.ylabel("tempo ÷ tempo do Floyd-Warshall")
legenda(2)
salvar("razao")

# 4: tempo dividido por n³ (custo de um passo), com a linha onde a matriz de distâncias
# (4n² bytes) deixa de caber no cache L2 ou no L3, se isso acontecer nos tamanhos medidos
plt.figure()
# deslocamento dos rótulos finais, para os do Floyd-Warshall e do heap não se encostarem
deslocamentos = {"floyd_warshall": -5, "dijkstra_v_vezes": 2, "dijkstra_vetor": 2}
for alg in ALGORITMOS:
    por_passo = [media[alg][i] / ns[i] ** 3 * 1e9 for i in range(len(ns))]
    plt.plot(ns, por_passo, marker=MARCADORES[alg], color=CORES[alg], label=NOMES[alg])
    rotulo(ultimo, por_passo[-1], com_virgula(por_passo[-1], 2), deslocamentos[alg])
eixo_x_log()
plt.ylim(0, 5)
for tamanho, nome in [(CACHE_L2, "L2"), (CACHE_L3, "L3")]:
    n_cache = math.sqrt(tamanho / 4)
    if n_cache <= ultimo:
        plt.axvline(n_cache, color=CINZA, linestyle="--", linewidth=1)
        plt.text(n_cache * 1.05, 4.75, "a matriz deixa de\ncaber no " + nome + f" (n ≈ {n_cache:.0f})",
                 va="top", fontsize=9, color=TEXTO)
plt.gca().yaxis.set_major_formatter(FuncFormatter(virgula))
plt.ylabel("tempo ÷ n³ (ns)")
legenda(3)
salvar("tempo_por_n3")

# 5: comparações divididas por n³
comparacoes = {"floyd_warshall": {}, "dijkstra_v_vezes": {}, "dijkstra_vetor": {}}
relaxamentos_dijkstra = {}
with open(os.path.join(PASTA, "contagens.csv")) as arquivo:
    for linha in csv.DictReader(arquivo):
        n = int(linha["n"])
        comparacoes[linha["algoritmo"]][n] = int(linha["comparacoes"])
        if linha["algoritmo"] == "dijkstra_v_vezes":
            relaxamentos_dijkstra[n] = int(linha["relaxamentos"])

plt.figure()
# deslocamento dos rótulos finais, para os do heap e dos relaxamentos não se sobreporem
deslocamentos = {"floyd_warshall": 0, "dijkstra_v_vezes": 4, "dijkstra_vetor": 0}
for alg in ALGORITMOS:
    valores = [comparacoes[alg][n] / n ** 3 for n in ns]
    plt.plot(ns, valores, marker=MARCADORES[alg], color=CORES[alg], label=NOMES[alg])
    rotulo(ultimo, valores[-1], com_virgula(valores[-1], 2) + " n³", deslocamentos[alg])
relax = [relaxamentos_dijkstra[n] / n ** 3 for n in ns]
plt.plot(ns, relax, marker="x", linestyle=":", color=CINZA, linewidth=1.5,
         label="só os relaxamentos (iguais nas duas versões)")
rotulo(ultimo, relax[-1], com_virgula(relax[-1], 2) + " n³", -5)
eixo_x_log()
plt.ylim(0, 1.4)
plt.gca().yaxis.set_major_formatter(FuncFormatter(virgula))
plt.ylabel("comparações ÷ n³")
legenda(2)
salvar("comparacoes_por_n3")

print("figuras gravadas em", FIGURAS)
