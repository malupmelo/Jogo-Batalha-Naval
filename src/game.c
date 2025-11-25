#include <stdio.h>
#include "game.h"

bool jogador_inicializar(Jogador *j, const char *apelido, int linhas, int colunas) {
    if (!j) return false;

    strncpy(j->apelido, apelido, sizeof(j->apelido) - 1);
    j->apelido[sizeof(j->apelido) - 1] = '\0';

    if (!tabuleiro_inicializar(&j->tabuleiro_navios, linhas, colunas))
        return false;

    if (!tabuleiro_inicializar(&j->mapa_tiros, linhas, colunas)) {
        tabuleiro_destruir(&j->tabuleiro_navios);
        return false;
    }

    if (!frota_inicializar(&j->frota)) {
        tabuleiro_destruir(&j->mapa_tiros);
        tabuleiro_destruir(&j->tabuleiro_navios);
        return false;
    }

    return true;
}

void jogador_destruir(Jogador *j) {
    if (!j) return;

    frota_destruir(&j->frota);
    tabuleiro_destruir(&j->mapa_tiros);
    tabuleiro_destruir(&j->tabuleiro_navios);
}

bool partida_inicializar(Partida *p, const char *apelido1, const char *apelido2,
                         int linhas, int colunas) {
    if (!p) return false;

    p->linhas  = linhas;
    p->colunas = colunas;
    p->partida_encerrada = false;
    p->jogador_atual = 1; // Jogador 1 começa

    if (!jogador_inicializar(&p->jogador1, apelido1, linhas, colunas))
        return false;

    if (!jogador_inicializar(&p->jogador2, apelido2, linhas, colunas)) {
        jogador_destruir(&p->jogador1);
        return false;
    }

    return true;
}

void partida_destruir(Partida *p) {
    if (!p) return;

    jogador_destruir(&p->jogador1);
    jogador_destruir(&p->jogador2);
}

bool game_posicionar_frota_automatica(Jogador *j) {
    if (!j) return false;

    Tabuleiro *t = &j->tabuleiro_navios;
    int total = t->linhas * t->colunas;

    for (int i = 0; i < total; i++) {
        t->celulas[i].estado = CELULA_AGUA;
        t->celulas[i].id_navio = -1;
    }

    return frota_posicionar_automatico(&j->frota, t);
}

ResultadoTiro game_tentar_tiro(Jogador *atirador, Jogador *alvo, int linha, int coluna) {
    Tabuleiro *navios = &alvo->tabuleiro_navios;
    Tabuleiro *tiros  = &atirador->mapa_tiros;

    int idx = tabuleiro_indice(navios, linha, coluna);

    if (tiros->celulas[idx].estado == CELULA_ACERTO ||
        tiros->celulas[idx].estado == CELULA_ERRO)
    {
        return TIRO_REPETIDO;
    }

    if (navios->celulas[idx].estado == CELULA_AGUA) {
        navios->celulas[idx].estado = CELULA_ERRO;
        tiros->celulas[idx].estado = CELULA_ERRO;
        return TIRO_AGUA;
    }

    if (navios->celulas[idx].estado == CELULA_NAVIO) {

        navios->celulas[idx].estado = CELULA_ACERTO;
        tiros->celulas[idx].estado = CELULA_ACERTO;

        int id = navios->celulas[idx].id_navio;
        frota_registrar_acerto(&alvo->frota, id);

        if (frota_navio_afundou(&alvo->frota, id)) {
            return TIRO_AFUNDOU;   
        }

        return TIRO_ACERTO;
    }

    return TIRO_INVALIDO;
}

bool game_frota_destruida(Jogador *j) {
    for (int i = 0; i < j->frota.quantidade; i++) {
        Navio *n = &j->frota.navios[i];
        if (n->acertos < n->tamanho)
            return false;
    }
    return true;
}


Jogador* partida_jogador_atual(Partida *p) {
    if (!p) return NULL;
    return (p->jogador_atual == 1 ? &p->jogador1 : &p->jogador2);
}

Jogador* partida_jogador_oponente(Partida *p) {
    if (!p) return NULL;
    return (p->jogador_atual == 1 ? &p->jogador2 : &p->jogador1);
}

void partida_trocar_turno(Partida *p) {
    if (!p) return;
    p->jogador_atual = (p->jogador_atual == 1 ? 2 : 1);
}