#ifndef PRODUTO_H_INCLUDED
#define PRODUTO_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

//Precisa adicionar o leProd
void imprimirBaseProd(FILE *out);

#endif
