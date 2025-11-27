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

    j->tiros_disparados = 0;
    j->tiros_acertados  = 0;

    

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
        io_ler_coordenada(&linha, &coluna, j->tabuleiro_navios.colunas);

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

    return frota_posicionar_automatico(&j->tabuleiro_navios, &j->frota);
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


    atirador->tiros_disparados++;

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

    
        atirador->tiros_acertados++;

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

    Jogador *atual    = partida_jogador_atual(p);
    Jogador *oponente = partida_jogador_oponente(p);

    int linha = 0, coluna = 0;

    printf("\n--- Turno de %s ---\n", atual->apelido);
    io_imprimir_duplo(atual);

    printf("\nDigite a coordenada do tiro:\n");
    io_ler_coordenada(&linha, &coluna, atual->tabuleiro_navios.colunas);

    ResultadoTiro r = game_tentar_tiro(atual, oponente, linha, coluna);

    switch (r) {
        case TIRO_INVALIDO:
            printf("Tiro inválido!\n");
            break;

        case TIRO_REPETIDO:
            printf("Você já atirou aí! Perdeu a vez.\n");
            break;

        case TIRO_AGUA:
            printf("Água!\n");
            break;

        case TIRO_ACERTO:
            printf("Acertou!\n");
            break;

        case TIRO_AFUNDOU:
            printf("Você afundou um navio!\n");
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

void game_imprimir_estatisticas(Jogador *j1, Jogador *j2) {
    printf("\n===== ESTATÍSTICAS DA PARTIDA =====\n");

    double precisao1 = 0.0;
    double precisao2 = 0.0;

    if (j1->tiros_disparados > 0) {
        precisao1 = (100.0 * j1->tiros_acertados) / j1->tiros_disparados;
    }

    if (j2->tiros_disparados > 0) {
        precisao2 = (100.0 * j2->tiros_acertados) / j2->tiros_disparados;
    }

    printf("\nJogador: %s\n", j1->apelido);
    printf("  Tiros disparados: %d\n", j1->tiros_disparados);
    printf("  Tiros acertados : %d\n", j1->tiros_acertados);
    printf("  Precisão        : %.1f%%\n", precisao1);

    printf("\nJogador: %s\n", j2->apelido);
    printf("  Tiros disparados: %d\n", j2->tiros_disparados);
    printf("  Tiros acertados : %d\n", j2->tiros_acertados);
    printf("  Precisão        : %.1f%%\n", precisao2);

    printf("===================================\n");
}

void game_mostrar_resultado_final(Partida *p) {
    printf("\n=== ESTADO FINAL DOS TABULEIROS ===\n");

    printf("\n--- Jogador: %s ---\n", p->jogador1.apelido);
    io_imprimir_duplo(&p->jogador1);

    printf("\n--- Jogador: %s ---\n", p->jogador2.apelido);
    io_imprimir_duplo(&p->jogador2);
}



void game_executar_partida(Partida *p) {
    printf("\n=== INICIANDO PARTIDA ===\n");

    while (1) {
        game_turno(p);


        if (game_frota_destruida(&p->jogador1)) {

            printf("\n=====================================\n");
            printf("             FIM DE JOGO\n");
            printf("=====================================\n");
            printf("Vencedor: %s\n", p->jogador2.apelido);

        
            printf("\n--- Tabuleiro final de %s ---\n", p->jogador1.apelido);
            io_imprimir_tabuleiro_completo(&p->jogador1);

            printf("\n--- Tabuleiro final de %s ---\n", p->jogador2.apelido);
            io_imprimir_tabuleiro_completo(&p->jogador2);

          
            printf("\n--- Estatísticas da partida ---\n");
            game_imprimir_estatisticas(&p->jogador1, &p->jogador2);

            printf("=====================================\n\n");

            return;
        }

        if (game_frota_destruida(&p->jogador2)) {

            printf("\n=====================================\n");
            printf("             FIM DE JOGO\n");
            printf("=====================================\n");
            printf("Vencedor: %s\n", p->jogador1.apelido);

            printf("\n--- Tabuleiro final de %s ---\n", p->jogador1.apelido);
            io_imprimir_tabuleiro_completo(&p->jogador1);

            printf("\n--- Tabuleiro final de %s ---\n", p->jogador2.apelido);
            io_imprimir_tabuleiro_completo(&p->jogador2);

            printf("\n--- Estatísticas da partida ---\n");
            game_imprimir_estatisticas(&p->jogador1, &p->jogador2);

            printf("=====================================\n\n");

            return;
        }
    }
}




void game_configuracoes(void) {
    int op;

    do {
        op = io_menu_configuracoes();

        if (op == 1) {
            printf("Novo apelido para Jogador 1: ");
            limparBuffer();
            lerString(current_config.nick1, sizeof(current_config.nick1));
            printf("Apelido alterado!\n");
        }
        else if (op == 2) {
            printf("Novo apelido para Jogador 2: ");
            limparBuffer();
            lerString(current_config.nick2, sizeof(current_config.nick2));
            printf("Apelido alterado!\n");
        }
        else if (op == 3) {
            int novo_tam;
            printf("Novo tamanho do tabuleiro (7 a 12): ");
            while (1) {
                if (scanf("%d", &novo_tam) != 1) {
                    limparBuffer();
                    printf("Valor inválido! Digite um número entre 7 e 12: ");
                    continue;
                }
                if (novo_tam < 7 || novo_tam > 12) {
                    printf("Valor inválido! Digite um número entre 7 e 12: ");
                    continue;
                }
                break;
            }
            current_config.tamanho_tabuleiro = novo_tam;
            printf("Tamanho do tabuleiro alterado para %d!\n", novo_tam);
        }
        else if (op == 4) {
            int escolha;

            printf("\n=== MODO DE POSICIONAMENTO ===\n");
            printf("1) Jogador 1 manual / Jogador 2 automático\n");
            printf("2) Jogador 1 automático / Jogador 2 automático\n");
            printf("3) Jogador 1 manual / Jogador 2 manual\n");
            printf("Escolha uma opção: ");

            while (1) {
                if (scanf("%d", &escolha) != 1) {
                    limparBuffer();
                    printf("Entrada inválida. Digite 1, 2 ou 3: ");
                    continue;
                }
                if (escolha < 1 || escolha > 3) {
                    printf("Opção inválida. Digite 1, 2 ou 3: ");
                    continue;
                }
                break;
            }

            if (escolha == 1) {
                current_config.posicao_manual_j1 = true;
                current_config.posicao_manual_j2 = false;
            } else if (escolha == 2) {
                current_config.posicao_manual_j1 = false;
                current_config.posicao_manual_j2 = false;
            } else { 
                current_config.posicao_manual_j1 = true;
                current_config.posicao_manual_j2 = true;
            }

            printf("Modo de posicionamento atualizado!\n");
        }

    } while (op != 5); 
}

void game_menu(void) {
    int opcao;

    while (1) {
        opcao = io_menu_principal();

        switch (opcao) {

        case 1: { 
            int tamanho = current_config.tamanho_tabuleiro;

            Partida p;

            if (!partida_inicializar(&p, current_config.nick1, current_config.nick2,
                                     tamanho, tamanho))
            {
                printf("Erro ao iniciar partida!\n");
                break;
            }

            printf("\n=== Posicionamento das Frotas ===\n");

        
            if (current_config.posicao_manual_j1) {
                printf("\n--- Jogador 1 (%s) posicionando manualmente ---\n",
                       p.jogador1.apelido);
                game_posicionar_frota_manual(&p.jogador1);
            } else {
                printf("\n--- Jogador 1 (%s) posicionamento automático ---\n",
                       p.jogador1.apelido);
                game_posicionar_frota_automatica(&p.jogador1);
            }

        
            if (current_config.posicao_manual_j2) {
                printf("\n--- Jogador 2 (%s) posicionando manualmente ---\n",
                       p.jogador2.apelido);
                game_posicionar_frota_manual(&p.jogador2);
            } else {
                printf("\n--- Jogador 2 (%s) posicionamento automático ---\n",
                       p.jogador2.apelido);
                game_posicionar_frota_automatica(&p.jogador2);
            }

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

