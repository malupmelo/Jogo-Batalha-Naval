#pragma once
#include <stdbool.h>

/* Estados possíveis de uma célula do tabuleiro */
typedef enum {
    CELULA_AGUA = 0,   // vazio, nunca atingida
    CELULA_NAVIO,      // contém parte de navio (não atingida)
    CELULA_ACERTO,     // tiro que acertou navio
    CELULA_ERRO        // tiro na água
} EstadoCelula;

/* Representa uma célula individual do tabuleiro */
typedef struct {
    EstadoCelula estado;
    int id_navio;      // -1 se não houver navio nesta célula
} Celula;

/* Tabuleiro do jogo, armazenado como vetor linear de células */
typedef struct {
    int linhas;        // recomendado: 6..26
    int colunas;       // recomendado: 6..26
    Celula *celulas;   // malloc(linhas * colunas)
} Tabuleiro;

/* Orientação para posicionamento de navios */
typedef enum {
    ORIENTACAO_HORIZONTAL = 0,
    ORIENTACAO_VERTICAL   = 1
} Orientacao;

/* Atalho para calcular índice linear (linha,coluna) -> índice do vetor */
static inline int tabuleiro_indice(const Tabuleiro *t, int linha, int coluna) {
    return linha * t->colunas + coluna;
}
