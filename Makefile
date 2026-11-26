# Makefile do 2º Trabalho de PAA - Grupo 8
#
#   make            compila os programas
#   make testes     roda os testes
#   make rodar      roda o experimento (os resultados vão para resultados/)
#   make graficos   gera as figuras em resultados/figuras/
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

clean:
	rm -rf build

.PHONY: all testes rodar graficos clean
