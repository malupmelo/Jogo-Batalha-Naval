#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "game.h"
#include "io.h"
#include "config.h"   
#include "board.h"
#include "fleet.h"


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


void game_posicionar_navio_manual(Jogador *j, Navio *n) {
    int linha = 0, coluna = 0;
    Orientacao o;

    while (1) {
        printf("\nPosicionando navio: %s (tamanho %d)\n",
               n->nome, n->tamanho);

        imprimir_tabuleiro_navios(&j->tabuleiro_navios);

        printf("\nEscolha a posição inicial (ex: A5):\n");
        io_ler_coordenada(&linha, &coluna);

        o = io_ler_orientacao();

        if (!frota_cabe_no_tabuleiro(&j->tabuleiro_navios, linha, coluna, n->tamanho, o)) {
            printf("O navio não cabe nessa posição!\n");
            continue;
        }

        if (frota_colide(&j->tabuleiro_navios, linha, coluna, n->tamanho, o)) {
            printf("O navio colide com outro!\n");
            continue;
        }

        int id_navio = (int)(n - j->frota.navios);
        frota_posicionar_navio(&j->tabuleiro_navios, &j->frota,
                               id_navio, linha, coluna, o);
        break;
    }

    imprimir_tabuleiro_navios(&j->tabuleiro_navios);
}

void game_posicionar_frota_manual(Jogador *j) {
    printf("\n=== Posicionamento manual de %s ===\n", j->apelido);

    for (int i = 0; i < j->frota.quantidade; i++) {
        game_posicionar_navio_manual(j, &j->frota.navios[i]);
    }

    printf("\nTodos os navios foram posicionados!\n");
}


bool game_posicionar_frota_automatica(Jogador *j) {
    if (!j) return false;

    Tabuleiro *t = &j->tabuleiro_navios;
    int total = t->linhas * t->colunas;

    for (int i = 0; i < total; i++) {
        t->celulas[i].estado = CELULA_AGUA;
        t->celulas[i].id_navio = -1;
    }

    return frota_posicionar_automatico(t, &j->frota);
}


ResultadoTiro game_tentar_tiro(Jogador *atirador, Jogador *alvo, int linha, int coluna) {
    if (!atirador || !alvo) return TIRO_INVALIDO;

    Tabuleiro *navios = &alvo->tabuleiro_navios;
    Tabuleiro *tiros  = &atirador->mapa_tiros;

    if (!tabuleiro_dentro_limites(navios, linha, coluna))
        return TIRO_INVALIDO;

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
    if (!j) return false;

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


void game_tela_vitoria(const char *vencedor) {
    printf("\n=====================================\n");
    printf("            FIM DE JOGO\n");
    printf("=====================================\n");
    printf("       O vencedor foi: %s\n", vencedor);
    printf("=====================================\n");
    printf("Pressione ENTER para voltar ao menu...");
    limparBuffer();
    getchar();
}

void game_turno(Partida *p) {
    if (!p) return;

    Jogador *atual   = partida_jogador_atual(p);
    Jogador *oponente = partida_jogador_oponente(p);

    int linha, coluna;

    printf("\n--- Turno de %s ---\n", atual->apelido);
    io_imprimir_duplo(atual);

    printf("\nDigite a coordenada do tiro:\n");
    io_ler_coordenada(&linha, &coluna);

    ResultadoTiro r = game_tentar_tiro(atual, oponente, linha, coluna);

    switch (r) {
        case TIRO_INVALIDO:
            printf("Tiro inválido! Tente novamente no seu próximo turno.\n");
            break;

        case TIRO_REPETIDO:
            printf("Você já atirou aí! Perdeu a vez.\n");
            break;

        case TIRO_AGUA:
            printf("Água! Nenhum navio atingido.\n");
            break;

        case TIRO_ACERTO:
            printf("Acertou um navio!\n");
            break;

        case TIRO_AFUNDOU:
            printf("Você afundou um navio inimigo!\n");
            break;
    }

    bool todos_afundados = true;
    for (int i = 0; i < oponente->frota.quantidade; i++) {
        if (!frota_navio_afundou(&oponente->frota, i)) {
            todos_afundados = false;
            break;
        }
    }

    if (todos_afundados) {
        game_tela_vitoria(atual->apelido);
        p->partida_encerrada = true;
        return;

    }

    partida_trocar_turno(p);
}

void game_executar_partida(Partida *p) {
    if (!p) return;

    p->partida_encerrada = false;

    printf("\nPreparando a partida...\n");
    printf("Jogador 1: %s\n", p->jogador1.apelido);
    printf("Jogador 2: %s\n\n", p->jogador2.apelido);

    while (!p->partida_encerrada) {
        game_turno(p);
    }

}


void game_configuracoes() {
    int op;

    do {
        op = io_menu_configuracoes();

        if (op == 1) {
            printf("Novo apelido para Jogador 1: ");
            limparBuffer();
            lerString(current_config.nick1, sizeof(current_config.nick1));
            printf("Apelido alterado!\n");
        } else if (op == 2) {
            printf("Novo apelido para Jogador 2: ");
            limparBuffer();
            lerString(current_config.nick2, sizeof(current_config.nick2));
            printf("Apelido alterado!\n");
        }

    } while (op != 3);
}

void game_menu() {
    int opcao;

    while (1) {
        opcao = io_menu_principal();

        switch (opcao) {

        case 1: {
            int linhas  = current_config.tamanho;
            int colunas = current_config.tamanho;

            Partida p;

            if (!partida_inicializar(&p, current_config.nick1, current_config.nick2,
                                     linhas, colunas))
            {
                printf("Erro ao iniciar partida!\n");
                break;
            }

            printf("\nPosicionando frota do %s manualmente...\n", p.jogador1.apelido);
            game_posicionar_frota_manual(&p.jogador1);

            printf("\nPosicionando frota do %s automaticamente...\n", p.jogador2.apelido);
            game_posicionar_frota_automatica(&p.jogador2);

            game_executar_partida(&p);
            partida_destruir(&p);
            break;
        }

        case 2:
            game_configuracoes();
            break;

        case 3:
            printf("\nSaindo do jogo...\n");
            return;  
        }
    }
}
