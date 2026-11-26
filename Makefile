# Makefile do 2º Trabalho de PAA - Grupo 8
#
#   make            compila os programas
#   make testes     roda os testes
#   make rodar      roda o experimento (os resultados vão para resultados/)
#   make graficos   gera as figuras em resultados/figuras/
#   make relatorio  compila o relatório (relatorio/relatorio.pdf)
#   make clean      apaga os arquivos gerados na compilação

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall
ARQUIVOS = src/grafo.cpp src/floyd_warshall.cpp src/dijkstra.cpp src/contagem.cpp

all: build/experimento build/testes

build/experimento: src/*.cpp src/*.hpp
	mkdir -p build
	$(CXX) $(CXXFLAGS) src/experimento.cpp $(ARQUIVOS) -o build/experimento

build/testes: src/*.cpp src/*.hpp
	mkdir -p build
	$(CXX) $(CXXFLAGS) src/testes.cpp $(ARQUIVOS) -o build/testes

testes: build/testes
	./build/testes

rodar: build/experimento
	mkdir -p resultados
	./build/experimento

graficos:
	python3 scripts/graficos.py

relatorio:
	cd relatorio && pdflatex -interaction=nonstopmode relatorio.tex && bibtex relatorio \
		&& pdflatex -interaction=nonstopmode relatorio.tex && pdflatex -interaction=nonstopmode relatorio.tex

clean:
	rm -rf build
	rm -f relatorio/*.aux relatorio/*.log relatorio/*.bbl relatorio/*.blg relatorio/*.out

.PHONY: all testes rodar graficos relatorio clean
