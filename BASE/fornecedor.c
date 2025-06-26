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
    // Lê a struct inteira de uma vez. Se a leitura falhar (fim do arquivo), retorna NULL.
    if (fread(fornec, sizeof(TFornecedor), 1, in) != 1) {
        free(fornec);
        return NULL;
    }
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

    while ((f = leFornec(out)) != NULL){
        imprimeFornec(f);
    }
    free(f);
}

//Vai ser o mesmo do cadastrar produto, praticamente
void cadastrarFornecedor(FILE *out) {
    char nome[100], cnpj[20], telefone[15];
    int temp_char;

    // Gera o id (mesma coisa do prod)
    int novo_id = 1;
    TFornecedor *f_temp;
    rewind(out); // Começa no começo
    while ((f_temp = leFornec(out)) != NULL) {
        if (f_temp->id >= novo_id) {
            novo_id = f_temp->id + 1;
        }
        free(f_temp); // Libera a memória alocada por le fornec
    }

    printf("\n### CADASTRO DE NOVO FORNECEDOR ###\n");
    printf("O ID do novo fornecedor será: %d\n", novo_id);

    // Pega os dados do fornecedor
    printf("Digite o nome do fornecedor: ");
    // Limpa o buffer
    int c;
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;

    printf("Digite o CNPJ do fornecedor: ");
    fgets(cnpj, sizeof(cnpj), stdin);
    cnpj[strcspn(cnpj, "\n")] = 0;

    printf("Digite o telefone do fornecedor: ");
    fgets(telefone, sizeof(telefone), stdin);
    telefone[strcspn(telefone, "\n")] = 0;

    // Salva no arquivo
    TFornecedor *f = fornecedor(novo_id, nome, cnpj, telefone);
    fseek(out, 0, SEEK_END); // Cursor no final
    salvaFornec(f, out);
    free(f);

    printf("\nFornecedor '%s' cadastrado com sucesso!\n", nome);
}
