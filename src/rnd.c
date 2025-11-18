#include <stdlib.h>
#include <time.h>
#include "rnd.h"

void aleatorio_inicializar(void) {
    srand((unsigned)time(NULL));
}

int aleatorio_intervalo(int minimo, int maximo) {
    return minimo + rand() % (maximo - minimo + 1);
}

