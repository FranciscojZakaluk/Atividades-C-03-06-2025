#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100 // Tamanho do vetor (solução)

// Função de avaliação: quanto maior a soma dos elementos, melhor
int avaliar(int solucao[N]) {
    int valor = 0;
    for (int i = 0; i < N; i++) {
        valor += solucao[i];
    }
    return valor;
}

// Gera uma nova solução vizinha alterando levemente a atual
void gerarVizinho(int atual[N], int vizinho[N]) {
    for (int i = 0; i < N; i++)
        vizinho[i] = atual[i];

    int pos = rand() % N;
    vizinho[pos] = rand() % 100; // Novo valor aleatório entre 0 e 99
}

int main() {
    int atual[N], vizinho[N];
    int valorAtual, valorVizinho;
    int melhorou = 1;

    srand(time(NULL)); // Semente para geração aleatória

    // Inicializa solução aleatória
    for (int i = 0; i < N; i++)
        atual[i] = rand() % 100;

    valorAtual = avaliar(atual);

    // Loop principal do Hill Climbing
    while (melhorou) {
        melhorou = 0;
        gerarVizinho(atual, vizinho);
        valorVizinho = avaliar(vizinho);

        if (valorVizinho > valorAtual) {
            for (int i = 0; i < N; i++)
                atual[i] = vizinho[i];
            valorAtual = valorVizinho;
            melhorou = 1;
        }
    }

    printf("Valor final (soma dos elementos): %d\n", valorAtual);
    return 0;
}
