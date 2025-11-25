#include <stdio.h>
#include "game.h"
#include "io.h"
#include "rnd.h"

int main(void) {
    aleatorio_inicializar();

    Partida p;

    // Valores provisórios (serão configuráveis na Sprint 3)
    int tamanho = 10;

    if (!partida_inicializar(&p, "Jogador1", "Jogador2", tamanho, tamanho)) {
        printf("Erro ao iniciar partida.\n");
        return 1;
    }

    printf("Partida criada com sucesso!\n");
    printf("Jogadores: %s vs %s\n", p.jogador1.apelido, p.jogador2.apelido);

    partida_destruir(&p);
    return 0;
}
