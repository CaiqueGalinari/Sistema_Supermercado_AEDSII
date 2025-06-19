#ifndef FORNECEDOR_H_INCLUDED
#define FORNECEDOR_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct fornecedor{
    int id;
    char nome[100];
    char cnpj[15];
    char telefone[12];
}TFornecedor;

TFornecedor *fornecedor(int id, char *nome, char *cnpj, char *telefone);

void salva(TFornecedor *fornecedor, FILE *out);

#endif