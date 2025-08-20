#ifndef MERCADO_H_INCLUDED
#define MERCADO_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fornecedor.h"
#include "produto.h"
#include "caixa.h"

extern int nCaixas;

void embaralha(int *vet, int tam);
void criarBaseMercado(int num_fornecedores, int num_produtos, int num_caixas, FILE* out_fornecedores, FILE* out_produtos, FILE* out_caixas, FILE* out_hash); //Hash aqui

void adicionarCaixas(FILE* arq_caixas, int num_a_adicionar);

TFornecedor *buscaBinariaFornecedor(int chave, FILE *in, int inicio, int fim, long *pos_encontrada, FILE *log);
//TProduto *buscaBinariaProduto(int chave, FILE *in, int inicio, int fim, long *pos_encontrada, FILE *log);
TCaixa *buscaSequencialCaixa(int chave, FILE *in, long *pos_encontrada, FILE *log);
void OrdenacaoEmDiscoFornec(FILE* arqEntrada, FILE* arqSaida, FILE* arqLog, int M);


void Vender(int id_caixa, int id_produto, FILE* arq_caixas, FILE* arq_produtos, FILE* arq_hash, FILE* log); //Hash aqui


//void consultarProdutoPorId(FILE *arq_produtos, int id, FILE *log);
void consultarFornecedorPorId(FILE *arq_fornecedores, int id, FILE *log);
void consultarCaixaPorId(FILE *arq_caixas, int id, FILE *log);

void atualizarProduto(FILE *arq_produtos, FILE *arq_fornecedores, FILE *arq_hash, FILE *log); //Hash aqui

void imprimirEstoqueBaixo(FILE *arquivo_produtos, int limite, FILE *log);

#endif
