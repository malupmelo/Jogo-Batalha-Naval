#include <stdio.h>
#include "board.h"
#include "fleet.h"
#include "rnd.h"

int main(void) {
    printf("=== BATALHA NAVAL ===\n");
    printf("1) Novo jogo\n2) Configurações\n3) Sair\n\n");

    aleatorio_inicializar();

    Tabuleiro t;
    Frota f;

    if (!tabuleiro_inicializar(&t, 10, 10)) {
        printf("ERRO: Falha ao inicializar tabuleiro!\n");
        return 1;
    }

    if (!frota_inicializar(&f)) {
        printf("ERRO: Falha ao inicializar frota!\n");
        tabuleiro_destruir(&t);
        return 1;
    }

    printf("Posicionando navios automaticamente...\n");

    if (frota_posicionar_automatico(&t, &f)) {
        printf("OK! Todos os navios foram posicionados.\n");
    } else {
        printf("ERRO: Não foi possível posicionar todos os navios.\n");
    }

    frota_destruir(&f);
    tabuleiro_destruir(&t);

    return 0;
}

