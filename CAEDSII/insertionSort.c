#include "funcionario.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

void insertionSort(FILE *arq, int tam, FILE *log) {
    Tmetrica metricas;
    metricas.comparacoes = 0;
    metricas.trocas = 0;
    metricas.texec = 0;
    clock_t inicio = clock();
    int i;
    double trocas = 0, comparacoes = 0; //Salvar no log*
    //faz o insertion sort
    for (int j = 2; j <= tam; j++) {
        metricas.comparacoes++;
        //posiciona o arquivo no registro j
        fseek(arq, (j-1) * tamanho_registro(), SEEK_SET);
        TFunc *fj = le(arq);
        i = j - 1;
        //posiciona o cursor no registro i
        fseek(arq, (i-1) * tamanho_registro(), SEEK_SET);
        TFunc *fi = le(arq);
        while ((i > 0) && (fi->cod > fj->cod)) {
            metricas.trocas++;
            //posiciona o cursor no registro i+1
            fseek(arq, i * tamanho_registro(), SEEK_SET);
            salva(fi, arq);
            i = i - 1;
            //lê registro i
            fseek(arq, (i-1) * tamanho_registro(), SEEK_SET);
            fi = le(arq);
        }
        //posiciona cursor no registro i + 1
        fseek(arq, (i) * tamanho_registro(), SEEK_SET);
        //salva registro j na posição i
        salva(fj, arq);
    }
    //descarrega o buffer para ter certeza que dados foram gravados
    fflush(arq);
    clock_t fim = clock();
    metricas.texec = (double)(fim - inicio) / CLOCKS_PER_SEC;
    fwrite(&metricas, sizeof(Tmetrica), 1, log);
}
