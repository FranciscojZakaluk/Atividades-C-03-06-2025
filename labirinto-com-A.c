#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define L 10
#define C 10

#define LIVRE '.'
#define PAREDE '#'
#define ABERTO 'O'
#define FECHADO 'X'
#define INICIO 'S'
#define OBJETIVO 'G'
#define CAMINHO '*'

typedef struct {
    int x, y;
} Ponto;

typedef struct {
    Ponto pos;
    int g, f;
    Ponto pai;
} No;

char labirinto[L][C] = {
    "S........#",
    "######...#",
    "#......#.#",
    "#.######.#",
    "#........#",
    "#.########",
    "#........#",
    "########.#",
    "#.......G#",
    "##########"
};

int explorados[L][C]; // 0: não explorado, 1: aberto, 2: fechado

int dx[] = { -1, 1, 0, 0 };
int dy[] = { 0, 0, -1, 1 };

int dentro(int x, int y) {
    return x >= 0 && x < L && y >= 0 && y < C;
}

// Heurísticas
int manhattan(Ponto a, Ponto b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

int euclidiana(Ponto a, Ponto b) {
    return (int)sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

// Heurística personalizada: Manhattan + penalidade por diagonais
int heuristica_custom(Ponto a, Ponto b) {
    return manhattan(a, b) + abs((a.x - b.x) - (a.y - b.y));
}

void mostrarLabirinto() {
    for (int i = 0; i < L; i++) {
        for (int j = 0; j < C; j++) {
            if (explorados[i][j] == 1 && labirinto[i][j] == LIVRE)
                printf("%c", ABERTO);
            else if (explorados[i][j] == 2 && labirinto[i][j] == LIVRE)
                printf("%c", FECHADO);
            else
                printf("%c", labirinto[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void reconstruir(No nos[L][C], Ponto atual) {
    while (labirinto[atual.x][atual.y] != INICIO) {
        if (labirinto[atual.x][atual.y] != OBJETIVO)
            labirinto[atual.x][atual.y] = CAMINHO;
        atual = nos[atual.x][atual.y].pai;
    }
}

void busca(Ponto inicio, Ponto objetivo, int (*heuristica)(Ponto, Ponto), float w) {
    No nos[L][C];
    int visitados = 0;

    for (int i = 0; i < L; i++)
        for (int j = 0; j < C; j++) {
            nos[i][j].f = INT_MAX;
            nos[i][j].g = INT_MAX;
            explorados[i][j] = 0;
        }

    nos[inicio.x][inicio.y].pos = inicio;
    nos[inicio.x][inicio.y].g = 0;
    nos[inicio.x][inicio.y].f = w * heuristica(inicio, objetivo);

    Ponto fila[L*C];
    int frente = 0, fim = 0;
    fila[fim++] = inicio;

    while (frente < fim) {
        // Encontra nó com menor f na fila
        int menor_idx = frente;
        for (int i = frente + 1; i < fim; i++) {
            Ponto p = fila[i];
            if (nos[p.x][p.y].f < nos[fila[menor_idx].x][fila[menor_idx].y].f)
                menor_idx = i;
        }
        Ponto atual = fila[menor_idx];
        fila[menor_idx] = fila[frente];
        frente++;

        explorados[atual.x][atual.y] = 2; // fechado
        visitados++;

        if (atual.x == objetivo.x && atual.y == objetivo.y) {
            reconstruir(nos, atual);
            printf("Caminho encontrado! Nós explorados: %d\n", visitados);
            mostrarLabirinto();
            return;
        }

        for (int d = 0; d < 4; d++) {
            int nx = atual.x + dx[d];
            int ny = atual.y + dy[d];
            if (!dentro(nx, ny)) continue;
            if (labirinto[nx][ny] == PAREDE) continue;

            int g = nos[atual.x][atual.y].g + 1;
            if (g < nos[nx][ny].g) {
                nos[nx][ny].g = g;
                nos[nx][ny].f = g + w * heuristica((Ponto){nx, ny}, objetivo);
                nos[nx][ny].pai = atual;

                if (explorados[nx][ny] != 1) {
                    fila[fim++] = (Ponto){nx, ny};
                    explorados[nx][ny] = 1; // aberto
                }
            }
        }
    }

    printf("Caminho não encontrado. Nós explorados: %d\n", visitados);
    mostrarLabirinto();
}

int main() {
    Ponto inicio, objetivo;
    for (int i = 0; i < L; i++)
        for (int j = 0; j < C; j++) {
            if (labirinto[i][j] == INICIO)
                inicio = (Ponto){i, j};
            else if (labirinto[i][j] == OBJETIVO)
                objetivo = (Ponto){i, j};
        }

    printf("Busca com Heurística de Manhattan (w = 1.0)\n");
    busca(inicio, objetivo, manhattan, 1.0);

    printf("Busca com Heurística Euclidiana (w = 1.0)\n");
    busca(inicio, objetivo, euclidiana, 1.0);

    printf("Busca com Heurística Personalizada (w = 1.0)\n");
    busca(inicio, objetivo, heuristica_custom, 1.0);

    printf("Busca com Manhattan (w = 2.0)\n");
    busca(inicio, objetivo, manhattan, 2.0);

    return 0;
}
