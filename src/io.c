#include "io.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "board.h"
#include "fleet.h"
#include <stdbool.h>
#include "game.h"

void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void lerString(char *str, int tamanho) {
    if (fgets(str, tamanho, stdin)) {
        int i = 0;
        while (str[i] != '\0') {
            if (str[i] == '\n') {
                str[i] = '\0';
                break;
            }
            i++;
        }

        if (str[i] != '\0')
            limparBuffer();
    }
}


int validarCoordenada(const char *entrada, int limite) {
    if (entrada == NULL || strlen(entrada) < 2)
        return 0;

    char letra = entrada[0];
    if (!isalpha(letra))
        return 0;

    letra = toupper(letra);

    if (letra < 'A' || letra >= 'A' + limite)
        return 0;

    int numero = atoi(entrada + 1);
    if (numero < 1 || numero > limite)
        return 0;

    return 1;
}

int converterCoordenada(const char *entrada, int *linha, int *coluna, int limite) {
    if (!validarCoordenada(entrada, limite))
        return 0;

    char letra = toupper(entrada[0]);
    *coluna = letra - 'A';

    int numero = atoi(entrada + 1);
    *linha = numero - 1;

    return 1;
}

void lerCoordenada(int *linha, int *coluna, int limite) {
    char entrada[16];

    while (1) {
        printf("Digite a coordenada (ex: A5): ");
        lerString(entrada, sizeof(entrada));

        if (converterCoordenada(entrada, linha, coluna, limite)) {
            return;
        }

        printf("Entrada inválida! Use letras A-%c e números entre 1-%d.\n",
               'A' + limite - 1, limite);
    }
}



void imprimir_tabuleiro_navios(const Tabuleiro *t) {
    if (!t) return;

    printf("\n    ");

    for (int c = 0; c < t->colunas; c++)
        printf(" %c ", 'A' + c);

    printf("\n");

    for (int r = 0; r < t->linhas; r++) {
        printf("%2d  ", r + 1);

        for (int c = 0; c < t->colunas; c++) {
            int i = tabuleiro_indice(t, r, c);

            char simbolo;
            switch (t->celulas[i].estado) {
                case CELULA_AGUA:   simbolo = '~'; break;
                case CELULA_NAVIO:  simbolo = '#'; break;
                case CELULA_ACERTO: simbolo = 'X'; break;
                case CELULA_ERRO:   simbolo = '·'; break;
                default: simbolo = '?';
            }

            printf(" %c ", simbolo);
        }
        printf("\n");
    }
}

void imprimir_mapa_tiros(const Tabuleiro *t) {
    if (!t) return;

    printf("\n    ");

    for (int c = 0; c < t->colunas; c++)
        printf(" %c ", 'A' + c);

    printf("\n");

    for (int r = 0; r < t->linhas; r++) {
        printf("%2d  ", r + 1);

        for (int c = 0; c < t->colunas; c++) {
            int i = tabuleiro_indice(t, r, c);

            char simbolo;

            if (t->celulas[i].estado == CELULA_ACERTO)
                simbolo = 'X';
            else if (t->celulas[i].estado == CELULA_ERRO)
                simbolo = '·';
            else
                simbolo = '~';

            printf(" %c ", simbolo);
        }
        printf("\n");
    }
}


int io_menu_principal(void) {
    int opcao = -1;

    printf("\n=== BATALHA NAVAL ===\n");
    printf("1) Novo jogo\n");
    printf("2) Configurações\n");
    printf("3) Sair\n");
    printf("Escolha uma opção: ");

    while (1) {
        if (scanf("%d", &opcao) != 1) {
            limparBuffer();
            printf("Entrada inválida. Digite 1, 2 ou 3: ");
            continue;
        }

        if (opcao < 1 || opcao > 3) {
            printf("Opção inexistente. Digite 1, 2 ou 3: ");
            continue;
        }

        break;
    }

    return opcao;
}


bool io_ler_coordenada(int *linha, int *coluna, int limite) {
    char entrada[8];

    printf("Digite a coordenada do tiro (ex: A5, C10): ");

    while (1) {
        if (scanf("%7s", entrada) != 1) {
            limparBuffer();
            printf("Entrada inválida. Tente novamente: ");
            continue;
        }

        if (converterCoordenada(entrada, linha, coluna, limite))
            return true;

        printf("Entrada inválida! Use letras A-%c e números entre 1-%d.\n",
               'A' + limite - 1, limite);
    }
}

int io_menu_configuracoes() {
    int op = -1;

    printf("\n=== CONFIGURAÇÕES ===\n");
    printf("1) Alterar apelido do Jogador 1\n");
    printf("2) Alterar apelido do Jogador 2\n");
    printf("3) Voltar ao menu principal\n");
    printf("Escolha: ");

    while (1) {
        if (scanf("%d", &op) != 1) {
            limparBuffer();
            printf("Entrada inválida. Digite 1, 2 ou 3: ");
            continue;
        }

        if (op < 1 || op > 3) {
            printf("Opção inválida. Digite 1, 2 ou 3: ");
            continue;
        }

        break;
    }

    return op;
}

Orientacao io_ler_orientacao() {
    char c;

    while (1) {
        printf("Orientação (H para horizontal, V para vertical): ");
        scanf(" %c", &c);
        c = toupper(c);

        if (c == 'H') return ORIENTACAO_HORIZONTAL;
        if (c == 'V') return ORIENTACAO_VERTICAL;

        printf("Entrada inválida! Use H ou V.\n");
    }
}

void io_imprimir_duplo(const Jogador *j) {
    const Tabuleiro *tiros = &j->mapa_tiros;
    const Tabuleiro *navios = &j->tabuleiro_navios;

    int linhas = tiros->linhas;
    int colunas = tiros->colunas;

    printf("\n================== VISUALIZAÇÃO ==================\n");
    printf(" ATAQUES                             SEU TABULEIRO\n\n");

    printf("    ");
    for (int c = 0; c < colunas; c++)
        printf(" %c ", 'A' + c);

    printf("          "); 

    for (int c = 0; c < colunas; c++)
        printf(" %c ", 'A' + c);

    printf("\n");

    for (int r = 0; r < linhas; r++) {
        printf("%2d  ", r + 1);

        for (int c = 0; c < colunas; c++) {
            int i = tabuleiro_indice(tiros, r, c);
            char simb =
                (tiros->celulas[i].estado == CELULA_ACERTO) ? 'X' :
                (tiros->celulas[i].estado == CELULA_ERRO)   ? '·' :
                                                             '~';
            printf(" %c ", simb);
        }

        printf("       "); 

        printf("%2d  ", r + 1);
        for (int c = 0; c < colunas; c++) {
            int i = tabuleiro_indice(navios, r, c);
            char simb =
                (navios->celulas[i].estado == CELULA_NAVIO)  ? '#' :
                (navios->celulas[i].estado == CELULA_ACERTO) ? 'X' :
                (navios->celulas[i].estado == CELULA_ERRO)   ? '·' :
                                                                '~';
            printf(" %c ", simb);
        }

        printf("\n");
    }

    printf("==================================================\n");
}

void io_limpar_tela() {
    printf("\033[2J\033[H");  
}
