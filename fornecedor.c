#include "fornecedor.h"
#include <string.h>

TFornecedor *fornecedor(int id, char *nome, char *cnpj, char *telefone){
    TFornecedor *fornec = (TFornecedor *) malloc(sizeof(TFornecedor));
    if (fornec) {
        memset(fornec, 0, sizeof(TFornecedor));
        fornec->id = id;
        strncpy(fornec->nome, nome, 100);
        strncpy(fornec->cnpj, cnpj, 15);
        strncpy(fornec->telefone, telefone, 12);
    }
    return fornec;
}

void salva(TFornecedor *fornecedor, FILE *out){
    fwrite(fornecedor, sizeof(TFornecedor), 1, out);
}

// criarBaseForn removida daqui