#include <stdio.h>
#include "game.h"
#include "rnd.h"
#include "config.h"

int main(void) {
    aleatorio_inicializar();   
    config_inicializar();      

    game_menu();               

    return 0;
}
