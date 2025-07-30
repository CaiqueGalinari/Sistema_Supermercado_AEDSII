#include "intercalacao_otima.h"
#include "fornecedor.h"
#include <stdlib.h>
#include <string.h>

#define MAX_PARTICOES 1000

typedef struct {
    char nomeArquivo[256];
    long numRegistros;
} ParticaoInfo;


static void minHeapify(ParticaoInfo arr[], int n, int i, TMetrica *metricas);
static ParticaoInfo extrairMin(ParticaoInfo arr[], int *n, TMetrica *metricas);
static void inserirNoHeap(ParticaoInfo arr[], int *n, ParticaoInfo novaParticao, TMetrica *metricas);
static ParticaoInfo mesclarParticoes(ParticaoInfo p1, ParticaoInfo p2, int idMerge, TMetrica* metricas);


void intercalacaoOtimaFornec(const char* nomeBase, int numParticoes, const char* nomeSaidaFinal, TMetrica* metricas) {
    if (numParticoes <= 0) return;
    if (numParticoes == 1) {
        char nomeParticao[256];
        sprintf(nomeParticao, "%s%d.dat", nomeBase, 0);
        rename(nomeParticao, nomeSaidaFinal);
        return;
    }

    ParticaoInfo heap[MAX_PARTICOES];
    int tamanhoHeap = 0;
    static int idMergeCounter = 0;

    for (int i = 0; i < numParticoes; i++) {
        char nomeParticao[256];
        sprintf(nomeParticao, "%s%d.dat", nomeBase, i);
        FILE *p = fopen(nomeParticao, "rb");
        fseek(p, 0, SEEK_END);
        long tam = ftell(p);
        fclose(p);

        heap[i].numRegistros = tam / sizeof(TFornecedor);
        strcpy(heap[i].nomeArquivo, nomeParticao);
        tamanhoHeap++;
    }

    for (int i = tamanhoHeap / 2 - 1; i >= 0; i--) {
        minHeapify(heap, tamanhoHeap, i, metricas);
    }

    while (tamanhoHeap > 1) {
        ParticaoInfo p1 = extrairMin(heap, &tamanhoHeap, metricas);
        ParticaoInfo p2 = extrairMin(heap, &tamanhoHeap, metricas);
        ParticaoInfo pMerge = mesclarParticoes(p1, p2, idMergeCounter++, metricas);
        inserirNoHeap(heap, &tamanhoHeap, pMerge, metricas);
    }

    rename(heap[0].nomeArquivo, nomeSaidaFinal);
}



static void minHeapify(ParticaoInfo arr[], int n, int i, TMetrica *metricas) {
    int menor = i;
    int esquerda = 2 * i + 1;
    int direita = 2 * i + 2;

    if (esquerda < n) {
        metricas->comparacoes++;
        if (arr[esquerda].numRegistros < arr[menor].numRegistros) menor = esquerda;
    }
    if (direita < n) {
        metricas->comparacoes++;
        if (arr[direita].numRegistros < arr[menor].numRegistros) menor = direita;
    }
    if (menor != i) {
        metricas->trocas++;
        ParticaoInfo temp = arr[i];
        arr[i] = arr[menor];
        arr[menor] = temp;
        minHeapify(arr, n, menor, metricas);
    }
}

static ParticaoInfo extrairMin(ParticaoInfo arr[], int *n, TMetrica *metricas) {
    metricas->trocas++;
    ParticaoInfo raiz = arr[0];
    arr[0] = arr[*n - 1];
    (*n)--;
    minHeapify(arr, *n, 0, metricas);
    return raiz;
}

static void inserirNoHeap(ParticaoInfo arr[], int *n, ParticaoInfo novaParticao, TMetrica *metricas) {
    (*n)++;
    int i = *n - 1;
    arr[i] = novaParticao;
    metricas->trocas++;

    while (i != 0) {
        metricas->comparacoes++;
        if(arr[(i - 1) / 2].numRegistros <= arr[i].numRegistros) break;
        metricas->trocas++;
        ParticaoInfo temp = arr[i];
        arr[i] = arr[(i - 1) / 2];
        arr[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}

static ParticaoInfo mesclarParticoes(ParticaoInfo p1, ParticaoInfo p2, int idMerge, TMetrica* metricas) {
    FILE *f1 = fopen(p1.nomeArquivo, "rb");
    FILE *f2 = fopen(p2.nomeArquivo, "rb");

    char nomeMerge[256];
    sprintf(nomeMerge, "merge_%d.dat", idMerge);
    FILE *fMerge = fopen(nomeMerge, "wb");

    TFornecedor *reg1 = leFornec(f1);
    TFornecedor *reg2 = leFornec(f2);
    long totalRegistros = 0;

    while (reg1 && reg2) {
        metricas->comparacoes++;
        if (reg1->id < reg2->id) {
            salvaFornec(reg1, fMerge);
            free(reg1);
            reg1 = leFornec(f1);
        } else {
            salvaFornec(reg2, fMerge);
            free(reg2);
            reg2 = leFornec(f2);
        }
        metricas->trocas++;
        totalRegistros++;
    }
    while (reg1) {
        salvaFornec(reg1, fMerge);
        free(reg1);
        reg1 = leFornec(f1);
        metricas->trocas++;
        totalRegistros++;
    }
    while (reg2) {
        salvaFornec(reg2, fMerge);
        free(reg2);
        reg2 = leFornec(f2);
        metricas->trocas++;
        totalRegistros++;
    }

    fclose(f1);
    fclose(f2);
    fclose(fMerge);
    remove(p1.nomeArquivo);
    remove(p2.nomeArquivo);

    ParticaoInfo pMerge;
    strcpy(pMerge.nomeArquivo, nomeMerge);
    pMerge.numRegistros = totalRegistros;
    return pMerge;
}
