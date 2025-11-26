#pragma once
#include <stdio.h>
#include <stdbool.h>
#include "board.h"   
#include "fleet.h"
#include "game.h"    

void limparBuffer(void);
void lerString(char *str, int tamanho);

int validarCoordenada(const char *entrada, int limite);
int converterCoordenada(const char *entrada, int *linha, int *coluna);
void lerCoordenada(int *linha, int *coluna, int limite);

void imprimir_tabuleiro_navios(const Tabuleiro *t);
void imprimir_mapa_tiros(const Tabuleiro *t);

int io_menu_principal(void);

bool io_ler_coordenada(int *linha, int *coluna);

void io_imprimir_duplo(const Jogador *j);
