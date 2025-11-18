#include <stdlib.h>
#include <string.h>
#include "fleet.h"
#include "board.h"

static void navio_definir(Navio *n, const char *nome, int tamanho) {
    strcpy(n->nome, nome);
    n->tamanho = tamanho;
    n->acertos = 0;
    n->posicionado = 0;
}

bool frota_inicializar(Frota *f) {
    if (!f) return false;

    f->quantidade = 6;
    f->navios = malloc(sizeof(Navio) * f->quantidade);
    if (!f->navios) return false;

    navio_definir(&f->navios[0], "Porta-Avioes", 5);
    navio_definir(&f->navios[1], "Encouracado", 4);
    navio_definir(&f->navios[2], "Cruzador A",  3);
    navio_definir(&f->navios[3], "Cruzador B",  3);
    navio_definir(&f->navios[4], "Destroyer A", 2);
    navio_definir(&f->navios[5], "Destroyer B", 2);

    return true;
}

void frota_destruir(Frota *f) {
    if (!f) return;
    free(f->navios);
    f->navios = NULL;
    f->quantidade = 0;
}

void frota_registrar_acerto(Frota *f, int id_navio) {
    if (!f || id_navio < 0 || id_navio >= f->quantidade) return;
    f->navios[id_navio].acertos++;
}

bool frota_navio_afundou(Frota *f, int id_navio) {
    if (!f || id_navio < 0 || id_navio >= f->quantidade) return false;
    Navio *n = &f->navios[id_navio];
    return n->acertos >= n->tamanho;
}

bool frota_cabe_no_tabuleiro(const Tabuleiro *t, int linha, int coluna, int tamanho, Orientacao orientacao) {
    if (!t) return false;

    if (orientacao == ORIENTACAO_HORIZONTAL) {
        return coluna + tamanho <= t->colunas;
    }

    else {
        return linha + tamanho <= t->linhas;
    }
}

bool frota_colide(const Tabuleiro *t, int linha, int coluna, int tamanho, Orientacao orientacao) {
    if (!t) return true; 

    for (int i = 0; i < tamanho; i++) {

        int r = linha;
        int c = coluna;

        if (orientacao == ORIENTACAO_HORIZONTAL)
            c += i;
        else
            r += i;

        if (!tabuleiro_dentro_limites(t, r, c))
            return true;

        int idx = tabuleiro_indice(t, r, c);

        if (t->celulas[idx].estado == CELULA_NAVIO)
            return true;
    }

    return false;
}

bool frota_posicionar_navio(Tabuleiro *t, Frota *f, int id_navio,
                            int linha, int coluna, Orientacao orientacao) {

    if (!t || !f || id_navio < 0 || id_navio >= f->quantidade)
        return false;

    Navio *n = &f->navios[id_navio];

    for (int i = 0; i < n->tamanho; i++) {

        int r = linha;
        int c = coluna;

        if (orientacao == ORIENTACAO_HORIZONTAL)
            c += i;
        else
            r += i;

        int idx = tabuleiro_indice(t, r, c);

        t->celulas[idx].estado   = CELULA_NAVIO;
        t->celulas[idx].id_navio = id_navio;
    }

    n->posicionado = 1;
    return true;
}






