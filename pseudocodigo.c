#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define LARGURA 10
#define ALTURA 10
#define MAX_NOS (LARGURA * ALTURA)
#define INFINITO 1000000

typedef struct {
    int x, y;
} Ponto;

typedef struct {
    Ponto pos;
    int g, f;
    Ponto pai;
    bool naListaAberta;
    bool naListaFechada;
} No;

int heuristica(Ponto a, Ponto b) {
    return abs(a.x - b.x) + abs(a.y - b.y); // Distância de Manhattan
}

int distancia(Ponto a, Ponto b) {
    return 1; // Para grid, o custo de mover para um vizinho é 1
}

bool pontosIguais(Ponto a, Ponto b) {
    return a.x == b.x && a.y == b.y;
}

void reconstruirCaminho(No nos[ALTURA][LARGURA], Ponto objetivo) {
    Ponto atual = objetivo;
    printf("Caminho: ");
    while (nos[atual.y][atual.x].pai.x != -1) {
        printf("(%d,%d) <- ", atual.x, atual.y);
        atual = nos[atual.y][atual.x].pai;
    }
    printf("INÍCIO\n");
}

void aEstrela(Ponto inicio, Ponto objetivo, bool obstaculos[ALTURA][LARGURA]) {
    No nos[ALTURA][LARGURA];

    // Inicializa todos os nós
    for (int y = 0; y < ALTURA; y++) {
        for (int x = 0; x < LARGURA; x++) {
            nos[y][x].pos = (Ponto){x, y};
            nos[y][x].g = INFINITO;
            nos[y][x].f = INFINITO;
            nos[y][x].pai = (Ponto){-1, -1};
            nos[y][x].naListaAberta = false;
            nos[y][x].naListaFechada = false;
        }
    }

    nos[inicio.y][inicio.x].g = 0;
    nos[inicio.y][inicio.x].f = heuristica(inicio, objetivo);
    nos[inicio.y][inicio.x].naListaAberta = true;

    while (1) {
        // Encontra o nó com menor f na lista aberta
        No *atual = NULL;
        for (int y = 0; y < ALTURA; y++) {
            for (int x = 0; x < LARGURA; x++) {
                if (nos[y][x].naListaAberta && (atual == NULL || nos[y][x].f < atual->f)) {
                    atual = &nos[y][x];
                }
            }
        }

        if (atual == NULL) {
            printf("Falha: Caminho não encontrado.\n");
            return;
        }

        if (pontosIguais(atual->pos, objetivo)) {
            reconstruirCaminho(nos, objetivo);
            return;
        }

        atual->naListaAberta = false;
        atual->naListaFechada = true;

        // Verifica vizinhos (cima, baixo, esquerda, direita)
        int dx[] = {0, 0, -1, 1};
        int dy[] = {-1, 1, 0, 0};

        for (int i = 0; i < 4; i++) {
            int nx = atual->pos.x + dx[i];
            int ny = atual->pos.y + dy[i];

            if (nx < 0 || ny < 0 || nx >= LARGURA || ny >= ALTURA) continue;
            if (obstaculos[ny][nx]) continue;

            No *vizinho = &nos[ny][nx];

            if (vizinho->naListaFechada) continue;

            int tentativaG = atual->g + distancia(atual->pos, vizinho->pos);

            if (!vizinho->naListaAberta || tentativaG < vizinho->g) {
                vizinho->pai = atual->pos;
                vizinho->g = tentativaG;
                vizinho->f = tentativaG + heuristica(vizinho->pos, objetivo);

                if (!vizinho->naListaAberta) {
                    vizinho->naListaAberta = true;
                }
            }
        }
    }
}
