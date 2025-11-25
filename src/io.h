#pragma once
#include <stdio.h>

void limparBuffer(void);
void lerString(char *str, int tamanho);

int validarCoordenada(const char *entrada, int limite);
int converterCoordenada(const char *entrada, int *linha, int *coluna);
void lerCoordenada(int *linha, int *coluna, int limite);
