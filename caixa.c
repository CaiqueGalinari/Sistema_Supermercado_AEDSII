#include "caixa.h"
#include <string.h>

TCaixa *caixa(int id, TListaprod *lista, int num_produtos) {
    TCaixa *caixaR = (TCaixa *) malloc(sizeof(TCaixa));
    if (caixaR) {
        memset(caixaR, 0, sizeof(TCaixa));
        caixaR->id = id;
        caixaR->lista = lista;
        caixaR->num_produtos = num_produtos;
    }
    return caixaR;
}

void salva(TCaixa *caixa, FILE *out) {
    fwrite(&caixa->id, sizeof(int), 1, out);
    fwrite(&caixa->num_produtos, sizeof(int), 1, out);
    fwrite(caixa->lista, sizeof(TListaprod), caixa->num_produtos, out);
}