#include <stdio.h>
#include "game.h"

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

int verificarAtaque(int linha, int coluna, char tabuleiroDefesa[10][10]) {
    if (linha < 0 || linha >= 10 || coluna < 0 || coluna >= 10) {
        return -1;
    }

    if (tabuleiroDefesa[linha][coluna] == 'X' || tabuleiroDefesa[linha][coluna] == 'O') {
        return -1;
    }

    if (tabuleiroDefesa[linha][coluna] == 'N') {
        tabuleiroDefesa[linha][coluna] = 'X';  
        return 1;
    }

    tabuleiroDefesa[linha][coluna] = 'O';  
    return 0;
}

void marcarAtaque(int linha, int coluna, int resultado, char tabuleiroAtaque[10][10]) {
    if (resultado == 1) {
        tabuleiroAtaque[linha][coluna] = 'X';  
    } else if (resultado == 0) {
        tabuleiroAtaque[linha][coluna] = 'O';  
    }
}
#include <stdio.h>
#include "game.h"

void lerCoordenadasAtaque(int *linha, int *coluna) {
    do {
        printf("Digite a linha (0-9): ");
        scanf("%d", linha);

        printf("Digite a coluna (0-9): ");
        scanf("%d", coluna);

        if (*linha < 0 || *linha > 9 || *coluna < 0 || *coluna > 9) {
            printf("Coordenadas inválidas! Tente novamente.\n");
        }

    } while (*linha < 0 || *linha > 9 || *coluna < 0 || *coluna > 9);
}

void realizarTurno(char tabuleiroDefesa[10][10], char tabuleiroAtaque[10][10]) {
    int linha, coluna;
    int resultado;

    do {
        lerCoordenadasAtaque(&linha, &coluna);

        resultado = verificarAtaque(linha, coluna, tabuleiroDefesa);

        if (resultado == -1) {
            printf("Posição já atacada ou inválida! Escolha outra.\n");
        }

    } while (resultado == -1);

    marcarAtaque(linha, coluna, resultado, tabuleiroAtaque);

    if (resultado == 1) {
        printf("Acertou!\n");
    } else if (resultado == 0) {
        printf("Água!\n");
    }
}

int verificarFimDeJogo(char tabuleiroDefesa[10][10]) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (tabuleiroDefesa[i][j] == 'N') {
                return 0; 
            }
        }
    }

    return 1;
}


ResultadoTiro game_atirar(Jogador *alvo, int linha, int coluna) {
    if (!alvo) return TIRO_INVALIDO;

    Tabuleiro *mapa = &alvo->mapa_tiros;
    Tabuleiro *navios = &alvo->tabuleiro_navios;

    if (!tabuleiro_dentro_limites(navios, linha, coluna))
        return TIRO_INVALIDO;

    int idx = tabuleiro_indice(navios, linha, coluna);

    if (mapa->celulas[idx].estado == CELULA_ACERTO ||
        mapa->celulas[idx].estado == CELULA_ERRO)
        return TIRO_REPETIDO;

    if (navios->celulas[idx].estado == CELULA_AGUA) {
        mapa->celulas[idx].estado = CELULA_ERRO;
        return TIRO_AGUA;
    }

    int id = navios->celulas[idx].id_navio;

    mapa->celulas[idx].estado = CELULA_ACERTO;

    frota_registrar_acerto(&alvo->frota, id);

    if (frota_navio_afundou(&alvo->frota, id))
        return TIRO_AFUNDOU;

    return TIRO_ACERTO;
}
