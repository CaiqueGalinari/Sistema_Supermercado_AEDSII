#ifndef MERCADO_H_INCLUDED
#define MERCADO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fornecedor.h"
#include "produto.h"
#include "caixa.h"

void criarBaseMercado(int num_fornecedores, int num_produtos, int num_caixas, FILE* out_fornecedores, FILE* out_produtos, FILE* out_caixas);

void adicionarCaixas(int num_caixas, FILE *out_caixas);

TProduto *buscaBinariaProduto(int chave, FILE *in, int inicio, int fim, long *pos_encontrada, FILE *log);

TCaixa *buscaSequencialCaixa(int chave, FILE *in, long *pos_encontrada, FILE *log);

void Vender(int id_caixa, int id_produto, FILE* arq_caixas, FILE* arq_produtos, FILE* log);

#endif
