#ifndef MERCADO_H_INCLUDED
#define MERCADO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fornecedor.h"
#include "produto.h"
#include "caixa.h"

void criarBaseMercado(int num_fornecedores, int num_produtos, int num_caixas, FILE* out_fornecedores, FILE* out_produtos, FILE* out_caixas);

#endif