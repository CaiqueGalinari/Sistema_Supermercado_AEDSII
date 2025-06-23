#ifndef CAIXA_H_INCLUDED
#define CAIXA_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "produto.h"

typedef struct lista_produtos{
    TProduto produto;
    int qtdvend;
    float valortotal;
}TListaprod;

typedef struct caixa{
    int id;
    int num_produtos;
    TListaprod *lista;
}TCaixa;

TCaixa *caixa(int id, TListaprod *lista, int num_produtos);

TCaixa *leCaixa(FILE *in);

void salvaCaixa(TCaixa *caixa, FILE *out);

void imprimeCaixa(TCaixa *caixa);

void imprimirBaseCaixa(FILE *out);

#endif
