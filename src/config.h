#pragma once
#include <stdbool.h>

typedef struct {
    char nick1[32];
    char nick2[32];
    int tamanho;    
} Configuracoes;

extern Configuracoes current_config;

void config_inicializar();

void game_configuracoes();
