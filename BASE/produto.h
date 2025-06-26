#ifndef PRODUTO_H_INCLUDED
#define PRODUTO_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fornecedor.h"

typedef struct produto{
    int id;
    char nome[100];
    char descricao[256];
    float preco;
    int estoque;
    int idFornecedor;
}TProduto;

TProduto *produto(int id, char *nome, char *descricao, float preco, int estoque, int idFornecedor);

TProduto *leProd(FILE *in);

void salvaProd(TProduto *produto, FILE *out);

void imprimeProd(TProduto *produto);

void imprimirBaseProd(FILE *out);

void cadastrarProduto(FILE *out_produtos, FILE *in_fornecedores);

#endif
