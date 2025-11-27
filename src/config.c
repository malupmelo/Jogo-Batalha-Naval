#include "config.h"
#include <string.h>

Configuracoes current_config; 

void config_inicializar(void) {
    strcpy(current_config.nick1, "Jogador1");
    strcpy(current_config.nick2, "Jogador2");
    current_config.tamanho_tabuleiro = 10;  

    current_config.posicao_manual_j1 = true;   
    current_config.posicao_manual_j2 = false;
}
