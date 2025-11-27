#pragma once
#include <stdbool.h>
#include "board.h"

typedef struct {
    char nome[20];   
    int tamanho;     
    int acertos;     
    int posicionado; 
} Navio;

typedef struct {
    Navio *navios;   
    int quantidade;  
} Frota;
bool frota_inicializar(Frota *f);
void frota_destruir(Frota *f);

void frota_registrar_acerto(Frota *f, int id_navio);
bool frota_navio_afundou(Frota *f, int id_navio);
bool frota_cabe_no_tabuleiro(const Tabuleiro *t, int linha, int coluna, int tamanho, Orientacao orientacao);
bool frota_colide(const Tabuleiro *t, int linha, int coluna, int tamanho, Orientacao orientacao);
bool frota_posicionar_navio(Tabuleiro *t, Frota *f, int id_navio,
                            int linha, int coluna, Orientacao orientacao);
int frota_linha_aleatoria(const Tabuleiro *t);
int frota_coluna_aleatoria(const Tabuleiro *t);
Orientacao frota_orientacao_aleatoria(void);
bool frota_posicionar_automatico(Tabuleiro *t, Frota *f);
int frota_navios_restantes(const Frota *f);







