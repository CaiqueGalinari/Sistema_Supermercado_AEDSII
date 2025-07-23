#ifndef SELECAO_NATURAL_H_INCLUDED
#define SELECAO_NATURAL_H_INCLUDED

#include <stdio.h>
#include "fornecedor.h"
#include "metricas.h"

int selecaoNaturalFornec(FILE* arqEntrada, const char* nomeBase, int M, TMetrica* metricas);

#endif
