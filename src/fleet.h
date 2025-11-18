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
bool frota_inicializar(Frota *f);
void frota_destruir(Frota *f);

/* Acertos e afundamento */
void frota_registrar_acerto(Frota *f, int id_navio);
bool frota_navio_afundou(Frota *f, int id_navio);
bool frota_cabe_no_tabuleiro(const Tabuleiro *t, int linha, int coluna, int tamanho, Orientacao orientacao);



