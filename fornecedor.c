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

// Le um fornecedor do arquivo in na posicao atual do cursor
// Retorna um ponteiro para funcionario lido do arquivo
TFornecedor *leFornec(FILE *in) {
    TFornecedor *fornec = (TFornecedor *) malloc(sizeof(TFornecedor));
    if (0 >= fread(&fornec->id, sizeof(int), 1, in)) {
        free(fornec);
        return NULL;
    }
    fread(fornec->nome, sizeof(char), sizeof(fornec->nome), in);
    fread(fornec->cnpj, sizeof(char), sizeof(fornec->cnpj), in);
    fread(fornec->telefone, sizeof(char), sizeof(fornec->telefone), in);
    fread(&fornec->id, sizeof(int), 1, in);
    return fornec;
}

void salvaFornec(TFornecedor *fornecedor, FILE *out){
    fwrite(fornecedor, sizeof(TFornecedor), 1, out);
}

void imprimeFornec(TFornecedor *fornecedor) {
    printf("\n**********************************************");
    printf("\nFornecedor de código: ");
    printf("%d", fornecedor->id);
    printf("\nNome: ");
    printf("%s", fornecedor->nome);
    printf("\nTelefone: ");
    printf("%s", fornecedor->telefone);
    printf("\nCNPJ: ");
    printf("%s", fornecedor->cnpj);
    printf("\n**********************************************");
}

void imprimirBaseFornec(FILE *out){
printf("\nImprimindo a base de dados de fornecedores...\n");

    rewind(out);
    TFornecedor *f;

    while ((f = leFornec(out)) != NULL)
        imprimeFornec(f);

    free(f);
}