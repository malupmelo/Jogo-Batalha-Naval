#include <stdlib.h>
#include "board.h"

#define MIN_DIM 6
#define MAX_DIM 26

bool tabuleiro_inicializar(Tabuleiro *t, int linhas, int colunas) {
    if (!t) return false;
    if (linhas < MIN_DIM || linhas > MAX_DIM) return false;
    if (colunas < MIN_DIM || colunas > MAX_DIM) return false;

    t->linhas  = linhas;
    t->colunas = colunas;

    size_t total = (size_t)linhas * (size_t)colunas;
    t->celulas = (Celula*)malloc(sizeof(Celula) * total);
    if (!t->celulas) {
        t->linhas = 0;
        t->colunas = 0;
        return false;
    }

    for (int r = 0; r < linhas; r++) {
        for (int c = 0; c < colunas; c++) {
            int i = tabuleiro_indice(t, r, c);
            t->celulas[i].estado   = CELULA_AGUA;
            t->celulas[i].id_navio = -1;
        }
    }
    return true;
}
void tabuleiro_destruir(Tabuleiro *t) {
    if (!t) return;             
    free(t->celulas);           
    t->celulas = NULL;          
    t->linhas = 0;              
    t->colunas = 0;
}

bool tabuleiro_dentro_limites(const Tabuleiro *t, int linha, int coluna) {
    if (!t || !t->celulas) return false;
    if (linha < 0 || coluna < 0) return false;
    if (linha >= t->linhas || coluna >= t->colunas) return false;
    return true;
}
