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
static void trocarCaix(FILE *arq, int i, int j, TMetrica *metricas) {
    if (i == j) return;
    TCaixa *f_i = (TCaixa*) malloc(sizeof(TCaixa));
    TCaixa *f_j = (TCaixa*) malloc(sizeof(TCaixa));

    fseek(arq, i * sizeof(TCaixa), SEEK_SET);
    fread(f_i, sizeof(TCaixa), 1, arq);
    fseek(arq, j * sizeof(TCaixa), SEEK_SET);
    fread(f_j, sizeof(TCaixa), 1, arq);

    fseek(arq, i * sizeof(TCaixa), SEEK_SET);
    fwrite(f_j, sizeof(TCaixa), 1, arq);
    fseek(arq, j * sizeof(TCaixa), SEEK_SET);
    fwrite(f_i, sizeof(TCaixa), 1, arq);

    metricas->trocas++;
    free(f_i);
    free(f_j);
}

static int particionaCaix(FILE *arq, int baixo, int alto, TMetrica *metricas) {
    TCaixa *pivo = (TCaixa*) malloc(sizeof(TCaixa));
    fseek(arq, alto * sizeof(TCaixa), SEEK_SET);
    fread(pivo, sizeof(TCaixa), 1, arq);

    int i = (baixo - 1);
    TCaixa *f_j = (TCaixa*) malloc(sizeof(TCaixa));
    for (int j = baixo; j <= alto - 1; j++) {
        fseek(arq, j * sizeof(TCaixa), SEEK_SET);
        fread(f_j, sizeof(TCaixa), 1, arq);
        metricas->comparacoes++;
        if (f_j->id < pivo->id) {
            i++;
            trocarCaix(arq, i, j, metricas);
        }
    }
    trocarCaix(arq, i + 1, alto, metricas);
    free(pivo);
    free(f_j);
    return (i + 1);
}

static void quicksortRecursivoCaix(FILE *arq, int baixo, int alto, TMetrica *metricas) {
    if (baixo < alto) {
        int pi = particionaCaix(arq, baixo, alto, metricas);
        quicksortRecursivoCaix(arq, baixo, pi - 1, metricas);
        quicksortRecursivoCaix(arq, pi + 1, alto, metricas);
    }
}

void quicksortCaix(FILE *arq, int tam, FILE *log) {
    if (tam <= 1) return;
    TMetrica metricas = {0.0, 0, 0};
    clock_t inicio = clock();

    quicksortRecursivoCaix(arq, 0, tam - 1, &metricas);

    clock_t fim = clock();
    metricas.texec = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    fflush(arq);

    fprintf(log, "\n--- Metricas Quicksort (Fornecedores) ---\n");
    fprintf(log, "Comparacoes: %d\n", metricas.comparacoes);
    fprintf(log, "Trocas: %d\n", metricas.trocas);
    fprintf(log, "Tempo de Execucao: %.4f segundos\n", metricas.texec);
    fflush(log);
}
