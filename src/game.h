#pragma once
#include <stdbool.h>
#include "board.h"
#include "fleet.h"

/* Resultado de um disparo (tiro) */
typedef enum {
    TIRO_INVALIDO = -1, // coordenada inválida / fora
    TIRO_REPETIDO = 0,  // célula já alvejada
    TIRO_AGUA     = 1,  // errou (água)
    TIRO_ACERTO   = 2,  // acertou navio (não afundou)
    TIRO_AFUNDOU  = 3   // acertou e afundou um navio
} ResultadoTiro;

/* Dados de um jogador */
typedef struct {
    Tabuleiro tabuleiro_navios; // onde ficam seus navios
    Tabuleiro mapa_tiros;       // mapa dos tiros que ele já deu
    Frota frota;                // sua frota
    char apelido[32];           // nome/apelido do jogador
} Jogador;

/* Estado geral da partida */
typedef struct {
    Jogador jogador1;
    Jogador jogador2;
    int jogador_atual;   // 1 ou 2
    int linhas;          // tamanho do tabuleiro
    int colunas;         // tamanho do tabuleiro
    bool partida_encerrada;
} Partida;


