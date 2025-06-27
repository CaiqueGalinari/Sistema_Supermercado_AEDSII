#ifndef METRICAS_H_INCLUDED
#define METRICAS_H_INCLUDED
#include <time.h>

typedef struct {
    double texec;       // Tempo de execução em segundos
    int comparacoes;    // Número de comparações
    int trocas;         // Número de trocas/movimentações
} TMetrica;

#endif
