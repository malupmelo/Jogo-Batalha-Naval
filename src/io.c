#include "io.h"
#include <stdio.h>

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

