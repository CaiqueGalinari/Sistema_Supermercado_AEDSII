#include "caixa.h"
#include <string.h>

TCaixa *caixa(int id, float valor_arrecadado) {
    TCaixa *caixaR = (TCaixa *) malloc(sizeof(TCaixa));
    if (caixaR) {
        caixaR->id = id;
        caixaR->valor_arrecadado = valor_arrecadado;
    }
    return caixaR;
}

TCaixa *leCaixa(FILE *in) {
    TCaixa *caixa = (TCaixa *) malloc(sizeof(TCaixa));
    if (fread(caixa, sizeof(TCaixa), 1, in) != 1) {
        free(caixa);
        return NULL;
    }
    return caixa;
}

void salvaCaixa(TCaixa *caixa, FILE *out) {
    fwrite(caixa, sizeof(TCaixa), 1, out);
}

void imprimeCaixa(TCaixa *caixa) {
    printf("\n**********************************************");
    printf("\nCaixa número: ");
    printf("%d", caixa->id);
    printf("\nValor arrecadado: ");
    printf("R$%.2f", caixa->valor_arrecadado);
    printf("\n**********************************************");
}

void imprimirBaseCaixa(FILE *out){
    printf("\nImprimindo a base de dados de caixas...\n");
    rewind(out);
    TCaixa *c;
    while ((c = leCaixa(out)) != NULL) {
        imprimeCaixa(c);
        free(c);
    }
}
