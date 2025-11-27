#pragma once
#include <stdbool.h>

typedef struct {
    char nick1[32];
    char nick2[32];
    int tamanho_tabuleiro;  
    
    bool posicao_manual_j1;  
    bool posicao_manual_j2;
} Configuracoes;

extern Configuracoes current_config;

void config_inicializar(void);

