# graficos.py - gera as figuras do relatório a partir dos CSVs da pasta resultados/
# Rodar de dentro da pasta codigo/: python3 scripts/graficos.py

import csv
import math
import os
import statistics

import matplotlib
matplotlib.use("Agg")   # só salva os arquivos, sem abrir janela
import matplotlib.pyplot as plt

PASTA = "resultados"
FIGURAS = os.path.join(PASTA, "figuras")
ALGORITMOS = ["floyd_warshall", "dijkstra_v_vezes"]
NOMES = {"floyd_warshall": "Floyd-Warshall", "dijkstra_v_vezes": "Dijkstra × V"}
MARCADORES = {"floyd_warshall": "o", "dijkstra_v_vezes": "s"}


def salvar(nome):
    plt.grid(True, alpha=0.3)
    plt.savefig(os.path.join(FIGURAS, nome + ".pdf"), bbox_inches="tight")
    plt.savefig(os.path.join(FIGURAS, nome + ".png"), dpi=150, bbox_inches="tight")
    plt.close()


# tempos[algoritmo][n] = lista com o tempo de cada repetição
tempos = {"floyd_warshall": {}, "dijkstra_v_vezes": {}}
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

# 1 e 2: tempo por execução em função de n, em escala linear e em escala log-log
for escala in ["linear", "loglog"]:
    plt.figure(figsize=(6.4, 4))
    for alg in ALGORITMOS:
        plt.errorbar(ns, media[alg], yerr=desvio[alg], marker=MARCADORES[alg], capsize=3, label=NOMES[alg])
    if escala == "loglog":
        plt.xscale("log")
        plt.yscale("log")
        # retas de referência n³ e n³·log n, passando pelo ponto do Floyd-Warshall no maior grafo
        t = media["floyd_warshall"][-1]
        maior = ns[-1]
        plt.plot(ns, [t * (n / maior) ** 3 for n in ns], "k:", label="referência n³")
        plt.plot(ns, [t * n ** 3 * math.log(n) / (maior ** 3 * math.log(maior)) for n in ns], "k--",
                 label="referência n³·log n")
    plt.xlabel("vértices (n)")
    plt.ylabel("tempo por execução (s)")
    plt.legend()
    salvar("tempo_" + escala)

# 3: razão entre os tempos (acima de 1, o Floyd-Warshall é mais rápido)
razao = [media["dijkstra_v_vezes"][i] / media["floyd_warshall"][i] for i in range(len(ns))]
plt.figure(figsize=(6.4, 4))
plt.plot(ns, razao, marker="D", color="gray")
plt.axhline(1, color="black", linewidth=1)
plt.xscale("log")
plt.xlabel("vértices (n)")
plt.ylabel("tempo do Dijkstra × V ÷ tempo do FW")
salvar("razao")

# 4: tempo dividido por n³ (custo de um passo), com as linhas onde a matriz de
# distâncias (4n² bytes) deixa de caber no cache L2 (256 KiB) e no L3 (6 MiB)
plt.figure(figsize=(6.4, 4))
for alg in ALGORITMOS:
    por_passo = [media[alg][i] / ns[i] ** 3 * 1e9 for i in range(len(ns))]
    plt.plot(ns, por_passo, marker=MARCADORES[alg], label=NOMES[alg])
for n_cache, texto in [(256, "matriz > L2"), (1254, "matriz > L3")]:
    plt.axvline(n_cache, color="gray", linestyle="--", linewidth=1)
    plt.text(n_cache, plt.ylim()[1], " " + texto, va="top", fontsize=8)
plt.xscale("log")
plt.xlabel("vértices (n)")
plt.ylabel("tempo ÷ n³ (ns)")
plt.legend(loc="center right")
salvar("tempo_por_n3")

# 5: comparações divididas por n³
comparacoes = {"floyd_warshall": {}, "dijkstra_v_vezes": {}}
relaxamentos_dijkstra = {}
with open(os.path.join(PASTA, "contagens.csv")) as arquivo:
    for linha in csv.DictReader(arquivo):
        n = int(linha["n"])
        comparacoes[linha["algoritmo"]][n] = int(linha["comparacoes"])
        if linha["algoritmo"] == "dijkstra_v_vezes":
            relaxamentos_dijkstra[n] = int(linha["relaxamentos"])

plt.figure(figsize=(6.4, 4))
for alg in ALGORITMOS:
    plt.plot(ns, [comparacoes[alg][n] / n ** 3 for n in ns], marker=MARCADORES[alg], label=NOMES[alg])
plt.plot(ns, [relaxamentos_dijkstra[n] / n ** 3 for n in ns], marker="^", linestyle=":",
         label="Dijkstra × V (só relaxamentos)")
plt.ylim(bottom=0)
plt.xscale("log")
plt.xlabel("vértices (n)")
plt.ylabel("comparações ÷ n³")
plt.legend()
salvar("comparacoes_por_n3")

print("figuras gravadas em", FIGURAS)
