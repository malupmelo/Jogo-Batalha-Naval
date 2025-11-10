#pragma once
#include <stdbool.h>

/* Representa um navio da frota */
typedef struct {
    char nome[20];   // ex.: "Porta-Aviões", "Cruzador"
    int tamanho;     // número de células ocupadas
    int acertos;     // quantas partes já foram acertadas
    int posicionado; // 0 = não; 1 = sim
} Navio;

/* Conjunto de navios de um jogador */
typedef struct {
    Navio *navios;   // vetor dinâmico
    int quantidade;  // tamanho do vetor
} Frota;

