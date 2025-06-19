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

void salva(TCaixa *caixa, FILE *out);

#endif