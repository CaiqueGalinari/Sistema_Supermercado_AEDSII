#include "produto.h"
#include <string.h>

TProduto *produto(int id, char *nome, char *descricao, float preco, int estoque, int idFornecedor){
    TProduto *prod = (TProduto *) malloc(sizeof(TProduto));
    if (prod) {
        memset(prod, 0, sizeof(TProduto));
        prod->id = id;
        strncpy(prod->nome, nome, 100);
        strncpy(prod->descricao, descricao, 256);
        prod->preco = preco;
        prod->estoque = estoque;
        prod->idFornecedor = idFornecedor;
    }
    return prod;
}

void salvaProd(TProduto *prod, FILE *out) {
    fwrite(prod, sizeof(TProduto), 1, out);
}

void imprimeProd(TProduto *produto) {
    printf("\n**********************************************");
    printf("\nProduto de código: ");
    printf("%d", produto->id);
    printf("\nNome: ");
    printf("%s", produto->nome);
    printf("\nDescrição: ");
    printf("%s", produto->descricao);
    printf("\nPreço unitário: ");
    printf("R$%.2lf", produto->preco);
    printf("\nFornecedor de código: ");
    printf("%d", produto->idFornecedor);
    printf("\n**********************************************");
}

//Precisa adicionar o leProd
void imprimirBaseProd(FILE *out){
printf("\nImprimindo a base de dados de produtos...\n");

    rewind(out);
    TProduto *p;

    while ((p = leProd(out)) != NULL)
        imprimeProd(p);

    free(p);
}