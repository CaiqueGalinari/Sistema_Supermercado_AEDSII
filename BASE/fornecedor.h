#ifndef FORNECEDOR_H_INCLUDED
#define FORNECEDOR_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct fornecedor{
    int id;
    char nome[100];
    char cnpj[20];
    char telefone[15];
}TFornecedor;

TFornecedor *fornecedor(int id, char *nome, char *cnpj, char *telefone);

TFornecedor *leFornec(FILE *in);

void salvaFornec(TFornecedor *fornecedor, FILE *out);

void imprimeFornec(TFornecedor *fornecedor);

void imprimirBaseFornec(FILE *out);

#endif
