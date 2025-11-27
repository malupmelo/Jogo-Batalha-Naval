#pragma once

#include <stdio.h>
#include <stdbool.h>
#include "board.h"
#include "fleet.h"
#include "game.h"

#define COR_RESET    "\033[0m"
#define COR_VERMELHO "\033[31m"
#define COR_VERDE    "\033[32m"
#define COR_AMARELO  "\033[33m"
#define COR_AZUL     "\033[34m"
#define COR_CIANO    "\033[36m"



void limparBuffer(void);
void lerString(char *str, int tamanho);


int validarCoordenada(const char *entrada, int limite);
int converterCoordenada(const char *entrada, int *linha, int *coluna, int limite);
void lerCoordenada(int *linha, int *coluna, int limite);
bool io_ler_coordenada(int *linha, int *coluna, int limite);


void imprimir_tabuleiro_navios(const Tabuleiro *t);
void imprimir_mapa_tiros(const Tabuleiro *t);
void io_imprimir_duplo(const Jogador *j);


int io_menu_principal(void);
int io_menu_configuracoes(void);


Orientacao io_ler_orientacao(void);


void io_limpar_tela(void);
void io_imprimir_tabuleiro_completo(const Jogador *j);

