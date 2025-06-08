#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 100
#define TEMP_INICIAL 1000.0
#define TEMP_MINIMA 0.01

// Avalia solução: soma dos elementos
int avaliar(int solucao[N]) {
    int valor = 0;
    for (int i = 0; i < N; i++) {
        valor += solucao[i];
    }
    return valor;
}

// Gera vizinho aleatório
void gerarVizinho(int atual[N], int vizinho[N]) {
    for (int i = 0; i < N; i++)
        vizinho[i] = atual[i];

    int pos = rand() % N;
    vizinho[pos] = rand() % 100;
}

// Simulated Annealing
double simulated_annealing(int solucao[N]) {
    double temp = TEMP_INICIAL;
    double taxa_resfriamento = 0.95;
    int solucao_atual[N], vizinho[N];
    double energia_atual, energia_vizinho;

    // Copia solução inicial
    for (int i = 0; i < N; i++)
        solucao_atual[i] = solucao[i];

    energia_atual = avaliar(solucao_atual);

    while (temp > TEMP_MINIMA) {
        gerarVizinho(solucao_atual, vizinho);
        energia_vizinho = avaliar(vizinho);

        if (energia_vizinho > energia_atual) {
            for (int i = 0; i < N; i++)
                solucao_atual[i] = vizinho[i];
            energia_atual = energia_vizinho;
        } else {
            double delta = energia_vizinho - energia_atual;
            double prob = exp(delta / temp);
            if ((double)rand() / RAND_MAX < prob) {
                for (int i = 0; i < N; i++)
                    solucao_atual[i] = vizinho[i];
                energia_atual = energia_vizinho;
            }
        }

        temp *= taxa_resfriamento;
    }

    // Copia solução final
    for (int i = 0; i < N; i++)
        solucao[i] = solucao_atual[i];

    return energia_atual;
}

// Exemplo de uso
int main() {
    int solucao[N];
    srand(time(NULL));

    // Inicializa solução inicial aleatória
    for (int i = 0; i < N; i++)
        solucao[i] = rand() % 100;

    double resultado = simulated_annealing(solucao);

    printf("Energia final (soma): %.2f\n", resultado);
    printf("Solução final: ");
    for (int i = 0; i < N; i++)
        printf("%d ", solucao[i]);
    printf("\n");

    return 0;
}
