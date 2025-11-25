#include <stdio.h>
#include "game.h"
#include "io.h"
#include "rnd.h"

int main() {
    aleatorio_inicializar();

    Partida p;
    partida_inicializar(&p, "J1", "J2", 10, 10);

    game_posicionar_frota_automatica(&p.jogador1);

    printf("Tabuleiro J1:\n");
    imprimir_tabuleiro_navios(&p.jogador1.tabuleiro_navios);

    partida_destruir(&p);
    return 0;
}