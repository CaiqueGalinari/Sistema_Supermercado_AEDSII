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

void salva(TProduto *prod, FILE *out) {
    fwrite(prod, sizeof(TProduto), 1, out);
}