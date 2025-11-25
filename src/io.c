#include "io.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void lerString(char *str, int tamanho) {
    if (fgets(str, tamanho, stdin)) {
        int i = 0;
        while (str[i] != '\0') {
            if (str[i] == '\n') {
                str[i] = '\0';
                break;
            }
            i++;
        }

        if (str[i] != '\0')
            limparBuffer();
    }
}


int validarCoordenada(const char *entrada, int limite) {
    if (entrada == NULL || strlen(entrada) < 2)
        return 0;

    char letra = entrada[0];

    if (!isalpha(letra))
        return 0;

    letra = toupper(letra);

    if (letra < 'A' || letra >= 'A' + limite)
        return 0;

    int numero = atoi(entrada + 1);

    if (numero < 1 || numero > limite)
        return 0;

    return 1;
}

int converterCoordenada(const char *entrada, int *linha, int *coluna) {
    if (!validarCoordenada(entrada, 10))
        return 0;

    char letra = toupper(entrada[0]);
    *coluna = letra - 'A';

    int numero = atoi(entrada + 1);
    *linha = numero - 1;

    return 1;
}

void lerCoordenada(int *linha, int *coluna, int limite) {
    char entrada[16];

    while (1) {
        printf("Digite a coordenada (ex: A5): ");
        lerString(entrada, sizeof(entrada));

        if (converterCoordenada(entrada, linha, coluna)) {
            return;
        }

        printf("Entrada inválida! Use letras A-%c e números entre 1-%d.\n",
               'A' + limite - 1, limite);
    }
}
