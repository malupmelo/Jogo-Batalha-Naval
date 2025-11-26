#include <stdio.h>
#include "game.h"
#include "io.h"

void partida_inicializar(Partida *p, const char *nome1, const char *nome2,
                         int linhas, int colunas)
{
    p->linhas = linhas;
    p->colunas = colunas;
    p->jogador_atual = 1;
    p->partida_encerrada = false;

    strcpy(p->jogador1.apelido, nome1);

    tabuleiro_inicializar(&p->jogador1.tabuleiro_navios, linhas, colunas);
    tabuleiro_inicializar(&p->jogador1.mapa_tiros, linhas, colunas);

    frota_inicializar(&p->jogador1.frota);

    strcpy(p->jogador2.apelido, nome2);

    tabuleiro_inicializar(&p->jogador2.tabuleiro_navios, linhas, colunas);
    tabuleiro_inicializar(&p->jogador2.mapa_tiros, linhas, colunas);

    frota_inicializar(&p->jogador2.frota);
}

void partida_destruir(Partida *p)
{
    tabuleiro_destruir(&p->jogador1.tabuleiro_navios);
    tabuleiro_destruir(&p->jogador1.mapa_tiros);
    frota_destruir(&p->jogador1.frota);

    tabuleiro_destruir(&p->jogador2.tabuleiro_navios);
    tabuleiro_destruir(&p->jogador2.mapa_tiros);
    frota_destruir(&p->jogador2.frota);
}


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
    p->jogador_atual = 1; 

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

void game_turno(Partida *p) {
    Jogador *atacante = (p->jogador_atual == 1 ? &p->jogador1 : &p->jogador2);
    Jogador *defensor = (p->jogador_atual == 1 ? &p->jogador2 : &p->jogador1);

    int linha, coluna;

    printf("\n--- Turno de %s ---\n", atacante->apelido);
    printf("Seu mapa de tiros:\n");
    imprimir_mapa_tiros(&atacante->mapa_tiros);

    printf("\nDigite a coordenada do tiro:\n");

    io_ler_coordenada(&linha, &coluna);

    ResultadoTiro r = game_tentar_tiro(atacante, defensor, linha, coluna);

    switch (r) {
        case TIRO_REPETIDO:
            printf("Você já atirou aqui! Perdeu a vez.\n");
            break;
        case TIRO_AGUA:
            printf("Água!\n");
            break;
        case TIRO_ACERTO:
            printf("Acertou um navio!\n");
            break;
        case TIRO_AFUNDOU:
            printf("Você afundou um navio inimigo!\n");
            break;
        default:
            printf("Erro no tiro.\n");
            break;
    }

    p->jogador_atual = (p->jogador_atual == 1 ? 2 : 1);
}

void game_executar_partida(Partida *p) {
    printf("\n=== INICIANDO PARTIDA ===\n");

    while (1) {
        game_turno(p);

        if (game_frota_destruida(&p->jogador1)) {
            printf("\nFIM DE JOGO! Jogador 2 venceu!\n");
            return;
        }

        if (game_frota_destruida(&p->jogador2)) {
            printf("\nFIM DE JOGO! Jogador 1 venceu!\n");
            return;
        }
    }
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


void game_menu() {
    int opcao;

    while (1) {
        opcao = io_menu_principal();

        if (opcao == 1) {
            // Novo jogo
            int linhas = 10;
            int colunas = 10;

            Partida p;
            partida_inicializar(&p, "Jogador 1", "Jogador 2", linhas, colunas);

            printf("\nPosicionando frotas automaticamente...\n");
            game_posicionar_frota_automatica(&p.jogador1);
            game_posicionar_frota_automatica(&p.jogador2);

            game_executar_partida(&p);
            partida_destruir(&p);
        }
        else if (opcao == 2) {
            printf("\n=== Configurações ===\n");
            printf("Função ainda não implementada.\n\n");
        }
        else if (opcao == 3) {
            printf("\nSaindo do jogo...\n");
            break;
        }
    }
}
