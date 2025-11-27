#include "config.h"
#include <string.h>

Configuracoes current_config; 

void config_inicializar() {
    strcpy(current_config.nick1, "Jogador1");
    strcpy(current_config.nick2, "Jogador2");
    current_config.tamanho = 10;  
}

void game_configuracoes() {
    printf("\nMenu de configurações ainda não implementado.\n");
}
