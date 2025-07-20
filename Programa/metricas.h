#ifndef METRICAS_H_INCLUDED
#define METRICAS_H_INCLUDED
#include <time.h>

typedef struct {
    double texec;       // Tempo de execu��o em segundos
    int comparacoes;    // N�mero de compara��es
    int trocas;         // N�mero de trocas/movimenta��es
} TMetrica;

#endif
