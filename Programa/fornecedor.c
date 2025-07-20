#include "fornecedor.h"
#include <string.h>
#include <time.h>

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

TFornecedor *leFornec(FILE *in) {
    TFornecedor *fornec = (TFornecedor *) malloc(sizeof(TFornecedor));
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
        free(f);
    }
}

void cadastrarFornecedor(FILE *out) {
    char nome[100], cnpj[20], telefone[15];
    int novo_id = 1;
    TFornecedor *f_temp;

    rewind(out);
    while ((f_temp = leFornec(out)) != NULL) {
        if (f_temp->id >= novo_id) {
            novo_id = f_temp->id + 1;
        }
        free(f_temp);
    }

    printf("\n### CADASTRO DE NOVO FORNECEDOR ###\n");
    printf("O ID do novo fornecedor será: %d\n", novo_id);

    printf("Digite o nome do fornecedor: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;

    printf("Digite o CNPJ do fornecedor: ");
    fgets(cnpj, sizeof(cnpj), stdin);
    cnpj[strcspn(cnpj, "\n")] = 0;

    printf("Digite o telefone do fornecedor: ");
    fgets(telefone, sizeof(telefone), stdin);
    telefone[strcspn(telefone, "\n")] = 0;

    TFornecedor *f = fornecedor(novo_id, nome, cnpj, telefone);
    fseek(out, 0, SEEK_END);
    salvaFornec(f, out);
    free(f);

    printf("\nFornecedor '%s' cadastrado com sucesso!\n", nome);
}

static void trocarFornec(FILE *arq, int i, int j, TMetrica *metricas) {
    if (i == j) return;
    TFornecedor *f_i = (TFornecedor*) malloc(sizeof(TFornecedor));
    TFornecedor *f_j = (TFornecedor*) malloc(sizeof(TFornecedor));

    fseek(arq, i * sizeof(TFornecedor), SEEK_SET);
    fread(f_i, sizeof(TFornecedor), 1, arq);
    fseek(arq, j * sizeof(TFornecedor), SEEK_SET);
    fread(f_j, sizeof(TFornecedor), 1, arq);

    fseek(arq, i * sizeof(TFornecedor), SEEK_SET);
    fwrite(f_j, sizeof(TFornecedor), 1, arq);
    fseek(arq, j * sizeof(TFornecedor), SEEK_SET);
    fwrite(f_i, sizeof(TFornecedor), 1, arq);

    metricas->trocas++;
    free(f_i);
    free(f_j);
}

static int particionaFornec(FILE *arq, int baixo, int alto, TMetrica *metricas) {
    TFornecedor *pivo = (TFornecedor*) malloc(sizeof(TFornecedor));
    fseek(arq, alto * sizeof(TFornecedor), SEEK_SET);
    fread(pivo, sizeof(TFornecedor), 1, arq);

    int i = (baixo - 1);
    TFornecedor *f_j = (TFornecedor*) malloc(sizeof(TFornecedor));
    for (int j = baixo; j <= alto - 1; j++) {
        fseek(arq, j * sizeof(TFornecedor), SEEK_SET);
        fread(f_j, sizeof(TFornecedor), 1, arq);
        metricas->comparacoes++;
        if (f_j->id < pivo->id) {
            i++;
            trocarFornec(arq, i, j, metricas);
        }
    }
    trocarFornec(arq, i + 1, alto, metricas);
    free(pivo);
    free(f_j);
    return (i + 1);
}

static void quicksortRecursivoFornec(FILE *arq, int baixo, int alto, TMetrica *metricas) {
    if (baixo < alto) {
        int pi = particionaFornec(arq, baixo, alto, metricas);
        quicksortRecursivoFornec(arq, baixo, pi - 1, metricas);
        quicksortRecursivoFornec(arq, pi + 1, alto, metricas);
    }
}

void quicksortFornec(FILE *arq, int tam, FILE *log) {
    if (tam <= 1) return;
    TMetrica metricas = {0.0, 0, 0};
    clock_t inicio = clock();

    quicksortRecursivoFornec(arq, 0, tam - 1, &metricas);

    clock_t fim = clock();
    metricas.texec = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    fflush(arq);

    fprintf(log, "\n--- Metricas Quicksort (Fornecedores) ---\n");
    fprintf(log, "Comparacoes: %d\n", metricas.comparacoes);
    fprintf(log, "Trocas: %d\n", metricas.trocas);
    fprintf(log, "Tempo de Execucao: %.4f segundos\n", metricas.texec);
    fflush(log);
}
