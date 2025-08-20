#ifndef PRODUTO_H_INCLUDED
#define PRODUTO_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fornecedor.h"
#include "metricas.h"

typedef struct produto{
    int id;
    char nome[100];
    char descricao[256];
    float preco;
    int estoque;
    int idFornecedor;
    long prox; //vai ter o endereço do proximo da lista em bytes
}TProduto;

typedef struct {
    long pont;
} THash;

//T3 COMEÇA AQUI
#define TAM_HASH_PROD 101

void criaHashProd(FILE *out);
int hashProd(int id);
long insereProdHash(TProduto *prod, FILE *arq_dados, FILE *arq_hash);
TProduto* buscaProdHash(int id, FILE *arq_dados, FILE *arq_hash, long *pos_encontrada);
void removeProdHash(int id, FILE *arq_dados, FILE *arq_hash);
void consultarProdutoPorIdHash(FILE *arq_produtos, FILE *arq_hash, int id, FILE *log);
//T3 ACABA AQUI

TProduto *produto(int id, char *nome, char *descricao, float preco, int estoque, int idFornecedor);

TProduto *leProd(FILE *in);
void salvaProd(TProduto *produto, FILE *out);

void imprimeProd(TProduto *produto);
void imprimirBaseProd(FILE *arq_dados, FILE *arq_hash);

//Comentados/alterados porque agora é hash
void cadastrarProduto(FILE *out_produtos, FILE *out_hash, FILE *in_fornecedores);
//void quicksortProd(FILE *arq, int tam, FILE *log);

#endif
