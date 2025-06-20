#include <stdio.h>
#include <stdlib.h>
#include "mercado.h"
#include "fornecedor.h"
#include "produto.h"
#include "caixa.h"


int main(){
    FILE *forn = fopen("fornecedores.dat", "wb+");
    FILE *prod = fopen("produtos.dat", "wb+");
    FILE *caix = fopen("caixas.dat", "wb+");

    criarBaseMercado(20, 50, 5, forn, prod, caix);

    imprimirBaseFornec(forn);
}