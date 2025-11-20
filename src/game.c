#include "game.h"
#include <stdio.h>

ResultadoTiro game_tentar_tiro(Partida *p, int linha, int coluna) {
    if (!p) return TIRO_INVALIDO;

    Jogador *atacante = (p->jogador_atual == 1 ? &p->jogador1 : &p->jogador2);
    Jogador *defensor = (p->jogador_atual == 1 ? &p->jogador2 : &p->jogador1);

    if (!tabuleiro_dentro_limites(&defensor->tabuleiro_navios, linha, coluna))
        return TIRO_INVALIDO;

    int idx = tabuleiro_indice(&defensor->tabuleiro_navios, linha, coluna);

    if (atacante->mapa_tiros.celulas[idx].estado == CELULA_ACERTO ||
        atacante->mapa_tiros.celulas[idx].estado == CELULA_ERRO)
    {
        return TIRO_REPETIDO;
    }

    Celula *cel = &defensor->tabuleiro_navios.celulas[idx];

    if (cel->estado == CELULA_AGUA) {
        atacante->mapa_tiros.celulas[idx].estado = CELULA_ERRO;
        cel->estado = CELULA_ERRO;
        return TIRO_AGUA;
    }

    if (cel->estado == CELULA_NAVIO) {
        atacante->mapa_tiros.celulas[idx].estado = CELULA_ACERTO;
        cel->estado = CELULA_ACERTO;

        int id = cel->id_navio;
        frota_registrar_acerto(&defensor->frota, id);

        if (frota_navio_afundou(&defensor->frota, id)) {
            return TIRO_AFUNDOU;
        }

        return TIRO_ACERTO;
    }

    return TIRO_INVALIDO; 
}


