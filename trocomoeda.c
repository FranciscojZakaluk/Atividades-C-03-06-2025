#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Algoritmo guloso
int troco_guloso(int moedas[], int n, int valor) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        while (valor >= moedas[i]) {
            valor -= moedas[i];
            count++;
        }
    }
    return (valor == 0) ? count : -1; // Retorna -1 se não conseguir troco exato
}

// Algoritmo exato com programação dinâmica
int troco_dinamico(int moedas[], int n, int valor) {
    int dp[valor + 1];
    for (int i = 0; i <= valor; i++) dp[i] = INT_MAX;
    dp[0] = 0;

    for (int i = 1; i <= valor; i++) {
        for (int j = 0; j < n; j++) {
            if (moedas[j] <= i && dp[i - moedas[j]] != INT_MAX) {
                if (dp[i] > dp[i - moedas[j]] + 1)
                    dp[i] = dp[i - moedas[j]] + 1;
            }
        }
    }

    return (dp[valor] == INT_MAX) ? -1 : dp[valor];
}

// Testa os dois algoritmos
void testar_troco(int moedas[], int n, int valor) {
    printf("\nMoedas: ");
    for (int i = 0; i < n; i++) printf("%d ", moedas[i]);
    printf("\nValor: %d\n", valor);

    int res_guloso = troco_guloso(moedas, n, valor);
    int res_dinamico = troco_dinamico(moedas, n, valor);

    printf("Guloso: ");
    if (res_guloso == -1)
        printf("Não encontrou solução exata.\n");
    else
        printf("%d moedas.\n", res_guloso);

    printf("Dinâmico (ótimo): %d moedas.\n", res_dinamico);
}

int comparar_decrescente(const void *a, const void *b) {
    return (*(int*)b - *(int*)a);
}

int main() {
    // Sistema brasileiro de moedas (em centavos)
    int moedas_brasil[] = {100, 50, 25, 10, 5};
    int n1 = sizeof(moedas_brasil) / sizeof(moedas_brasil[0]);
    qsort(moedas_brasil, n1, sizeof(int), comparar_decrescente);

    testar_troco(moedas_brasil, n1, 370); // 3.70 reais

    // Contraexemplo para abordagem gulosa
    int moedas_contraexemplo[] = {4, 3, 1};
    int n2 = sizeof(moedas_contraexemplo) / sizeof(moedas_contraexemplo[0]);
    qsort(moedas_contraexemplo, n2, sizeof(int), comparar_decrescente);

    testar_troco(moedas_contraexemplo, n2, 6); // Guloso falha aqui (4+1+1 = 3 moedas), ótimo seria 3+3 = 2 moedas

    return 0;
}
