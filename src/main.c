#include <stdio.h>
#include "board.h"
#include "fleet.h"

int main(void) {
    printf("=== BATALHA NAVAL ===\n");
    printf("(menu em construção)\n");
    printf("1) Novo jogo\n2) Configurações\n3) Sair\n");

    Tabuleiro t;
    if (tabuleiro_inicializar(&t, 10, 10)) {
        printf("Tabuleiro 10x10 OK\n");
        printf("(0,0) dentro? %s\n",  tabuleiro_dentro_limites(&t, 0, 0) ? "sim" : "nao");
        printf("(9,9) dentro? %s\n",  tabuleiro_dentro_limites(&t, 9, 9) ? "sim" : "nao");
        printf("(10,10) dentro? %s\n", tabuleiro_dentro_limites(&t, 10, 10) ? "sim" : "nao");
        tabuleiro_destruir(&t);
    } else {
        printf("Falha ao inicializar tabuleiro\n");
    }
    Frota f;
    if (frota_inicializar(&f)) {
    printf("Frota inicializada com %d navios.\n", f.quantidade);
    frota_destruir(&f);
    }

    return 0;
}
