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

TCaixa *leCaixa(FILE *in) {
    TCaixa *caixa = (TCaixa *) malloc(sizeof(TCaixa));

    fread(&caixa->id, sizeof(int), 1, in);
    fread(&caixa->num_produtos, sizeof(int), 1, in);
    if (caixa->num_produtos > 0) {
        caixa->lista = (TListaprod *) malloc(sizeof(TListaprod) * caixa->num_produtos);
        fread(caixa->lista, sizeof(TListaprod), caixa->num_produtos, in);
    } else {
        caixa->lista = NULL;
    }
    return caixa;
}

void salvaCaixa(TCaixa *caixa, FILE *out) {
    fwrite(&caixa->id, sizeof(int), 1, out);
    fwrite(&caixa->num_produtos, sizeof(int), 1, out);
    fwrite(caixa->lista, sizeof(TListaprod), caixa->num_produtos, out);
}

void imprimeCaixa(TCaixa *caixa) {
    printf("\n**********************************************");
    printf("\nCaixa número ");
    printf("%d", caixa->id);
    printf("\nValor arrecadado: ");
    printf("%f", caixa->lista->valortotal);
    printf("\n**********************************************");
}

//Precisa adicionar o leCaixa
void imprimirBaseCaixa(FILE *out){
    printf("\nImprimindo a base de dados de caixas...\n");
    rewind(out);
    TCaixa *c;

    while ((c = leCaixa(out)) != NULL) {
        imprimeCaixa(c); // CORREÇÃO: Chamando a função correta
        // Libera a memória da lista de produtos e do caixa
        if (c->lista) {
            free(c->lista);
        }
        free(c);
    }
}
