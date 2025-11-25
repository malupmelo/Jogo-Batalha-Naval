#pragma once
#include <stdbool.h>
#include "board.h"
#include "fleet.h"

typedef enum {
    TIRO_INVALIDO = -1, 
    TIRO_REPETIDO = 0,  
    TIRO_AGUA     = 1,  
    TIRO_ACERTO   = 2,  
    TIRO_AFUNDOU  = 3   
} ResultadoTiro;

typedef struct {
    Tabuleiro tabuleiro_navios; 
    Tabuleiro mapa_tiros;       
    Frota frota;                
    char apelido[32];           
} Jogador;

typedef struct {
    Jogador jogador1;
    Jogador jogador2;
    int jogador_atual;   
    int linhas;          
    int colunas;         
    bool partida_encerrada;
} Partida;

bool jogador_inicializar(Jogador *j, const char *apelido, int linhas, int colunas);
void jogador_destruir(Jogador *j);

bool partida_inicializar(Partida *p, const char *apelido1, const char *apelido2,
                         int linhas, int colunas);
void partida_destruir(Partida *p);

ResultadoTiro game_tentar_tiro(Jogador *atirador, Jogador *alvo, int linha, int coluna);
Jogador* partida_jogador_atual(Partida *p);
Jogador* partida_jogador_oponente(Partida *p);
void partida_trocar_turno(Partida *p);
int verificarAtaque(int linha, int coluna, char tabuleiroDefesa[10][10]);
void marcarAtaque(int linha, int coluna, int resultado, char tabuleiroAtaque[10][10]);
void lerCoordenadasAtaque(int *linha, int *coluna);
void realizarTurno(char tabuleiroDefesa[10][10], char tabuleiroAtaque[10][10]);
int verificarFimDeJogo(char tabuleiroDefesa[10][10]);

ResultadoTiro game_atirar(Jogador *alvo, int linha, int coluna);




