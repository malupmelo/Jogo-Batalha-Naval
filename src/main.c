#include <stdio.h>
#include "board.h"
#include "fleet.h"
#include "game.h"
#include "io.h"
#include "rnd.h"

int main() {
    aleatorio_inicializar();

    Partida p;
    partida_inicializar(&p, "Jogador 1", "Jogador 2", 10, 10);

    printf("Posicionando frota automaticamente...\n");

    game_posicionar_frota_automatica(&p.jogador1);
    game_posicionar_frota_automatica(&p.jogador2);

    game_executar_partida(&p);

    partida_destruir(&p);
    return 0;
}
