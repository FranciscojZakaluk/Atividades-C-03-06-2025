#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N_SEQUENCIAS 3
#define TAM_SEQUENCIA 8
#define ITERACOES 1000

char sequencias[N_SEQUENCIAS][TAM_SEQUENCIA + 1] = {
    "ACGTACGT",
    "ACTTACGG",
    "ATGTACCT"
};

// Calcula a distância de Hamming entre duas sequências
int hamming(const char *a, const char *b) {
    int dist = 0;
    for (int i = 0; i < TAM_SEQUENCIA; i++) {
        if (a[i] != b[i])
            dist++;
    }
    return dist;
}

// Avalia uma sequência candidata: soma das distâncias para todas
int avaliar(const char *candidato) {
    int total = 0;
    for (int i = 0; i < N_SEQUENCIAS; i++) {
        total += hamming(candidato, sequencias[i]);
    }
    return total;
}

// Método da maioria por coluna
void consenso_maioria(char *resultado) {
    char bases[] = {'A', 'C', 'G', 'T'};
    for (int i = 0; i < TAM_SEQUENCIA; i++) {
        int contagem[4] = {0};
        for (int j = 0; j < N_SEQUENCIAS; j++) {
            char base = sequencias[j][i];
            for (int b = 0; b < 4; b++) {
                if (base == bases[b]) contagem[b]++;
            }
        }
        // escolhe base com maior contagem
        int max_idx = 0;
        for (int b = 1; b < 4; b++) {
            if (contagem[b] > contagem[max_idx]) max_idx = b;
        }
        resultado[i] = bases[max_idx];
    }
    resultado[TAM_SEQUENCIA] = '\0';
}

// Gera uma sequência vizinha modificando uma posição aleatória
void gerarVizinho(char *atual, char *vizinho) {
    strcpy(vizinho, atual);
    int pos = rand() % TAM_SEQUENCIA;
    char bases[] = {'A', 'C', 'G', 'T'};
    char nova;
    do {
        nova = bases[rand() % 4];
    } while (nova == atual[pos]); // evita repetir
    vizinho[pos] = nova;
}

// Hill Climbing para encontrar uma boa sequência
void hill_climbing(char *solucao_final) {
    char atual[TAM_SEQUENCIA + 1], vizinho[TAM_SEQUENCIA + 1];
    // Inicializa aleatoriamente
    char bases[] = {'A', 'C', 'G', 'T'};
    for (int i = 0; i < TAM_SEQUENCIA; i++) {
        atual[i] = bases[rand() % 4];
    }
    atual[TAM_SEQUENCIA] = '\0';

    int valorAtual = avaliar(atual);

    for (int iter = 0; iter < ITERACOES; iter++) {
        gerarVizinho(atual, vizinho);
        int valorVizinho = avaliar(vizinho);

        if (valorVizinho < valorAtual) {
            strcpy(atual, vizinho);
            valorAtual = valorVizinho;
        }
    }

    strcpy(solucao_final, atual);
}

int main() {
    srand(time(NULL));

    char consenso1[TAM_SEQUENCIA + 1];
    char consenso2[TAM_SEQUENCIA + 1];

    consenso_maioria(consenso1);
    hill_climbing(consenso2);

    printf("Sequências:\n");
    for (int i = 0; i < N_SEQUENCIAS; i++)
        printf("%s\n", sequencias[i]);

    printf("\nConsenso por maioria:  %s (score: %d)\n", consenso1, avaliar(consenso1));
    printf("Consenso Hill Climb:   %s (score: %d)\n", consenso2, avaliar(consenso2));

    return 0;
}
