#include <stdio.h>
#include "game.h"
#include "rnd.h"
#include "config.h"

int main() {
    aleatorio_inicializar();

    game_menu();

    return 0;
}

int main() {
    config_inicializar();
    game_menu();
}
