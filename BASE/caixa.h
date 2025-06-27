#ifndef CAIXA_H_INCLUDED
#define CAIXA_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "produto.h"
#include "metricas.h"

typedef struct caixa{
    int id;
    float valor_arrecadado;
}TCaixa;

TCaixa *caixa(int id, float valor_arrecadado);

TCaixa *leCaixa(FILE *in);
void salvaCaixa(TCaixa *caixa, FILE *out);

void imprimeCaixa(TCaixa *caixa);
void imprimirBaseCaixa(FILE *out);

void quicksortCaix(FILE *arq, int tam, FILE *log);



#endif
