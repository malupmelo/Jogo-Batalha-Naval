#pragma once
#include <stdbool.h>

typedef enum {
    CELULA_AGUA = 0,   
    CELULA_NAVIO,      
    CELULA_ACERTO,     
    CELULA_ERRO        
} EstadoCelula;

typedef struct {
    EstadoCelula estado;
    int id_navio;      
} Celula;

typedef struct {
    int linhas;        
    int colunas;       
    Celula *celulas;   
} Tabuleiro;

typedef enum {
    ORIENTACAO_HORIZONTAL = 0,
    ORIENTACAO_VERTICAL   = 1
} Orientacao;

static inline int tabuleiro_indice(const Tabuleiro *t, int linha, int coluna) {
    return linha * t->colunas + coluna;
}


bool tabuleiro_inicializar(Tabuleiro *t, int linhas, int colunas);
void tabuleiro_destruir(Tabuleiro *t);
bool tabuleiro_dentro_limites(const Tabuleiro *t, int linha, int coluna);




