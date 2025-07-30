#include "selecao_natural.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int selecaoNaturalFornec(FILE* arqEntrada, const char* nomeBase, int M, TMetrica* metricas) {
    rewind(arqEntrada);
    int numParticoes = 0;
    TFornecedor* memoria = (TFornecedor*) malloc(M * sizeof(TFornecedor));
    TFornecedor* reservatorio = (TFornecedor*) malloc(M * sizeof(TFornecedor));
    int posMem = 0, posRes = 0;

    for (posMem = 0; posMem < M; posMem++) {
        TFornecedor *f = leFornec(arqEntrada);
        if (f) {
            memoria[posMem] = *f;
            free(f);
            metricas->trocas++;
        } else {
            break;
        }
    }

    if(posMem == 0){
        free(memoria);
        free(reservatorio);
        return 0;
    }

    int registrosNaMemoria = posMem;


    while (registrosNaMemoria > 0) {
        char nomeParticao[256];
        sprintf(nomeParticao, "%s%d.dat", nomeBase, numParticoes);
        FILE* arqParticao = fopen(nomeParticao, "wb");
        numParticoes++;

        int idUltimoEscrito = -1;


        while (registrosNaMemoria > 0) {
            int menorId = INT_MAX;
            int indiceMenor = -1;


            for (int i = 0; i < M; i++) {
                if(memoria[i].id != -1){
                    metricas->comparacoes++;
                    if (memoria[i].id >= idUltimoEscrito && memoria[i].id < menorId) {
                        menorId = memoria[i].id;
                        indiceMenor = i;
                    }
                }
            }

            if (indiceMenor == -1) {
                break;
            }


            TFornecedor menorFornecedor = memoria[indiceMenor];
            salvaFornec(&menorFornecedor, arqParticao);
            metricas->trocas++;
            idUltimoEscrito = menorFornecedor.id;


            TFornecedor* novoReg = leFornec(arqEntrada);
            if (novoReg) {
                metricas->comparacoes++;
                if (novoReg->id >= idUltimoEscrito) {

                    memoria[indiceMenor] = *novoReg;
                } else {

                    reservatorio[posRes++] = *novoReg;
                    memoria[indiceMenor].id = -1;
                    registrosNaMemoria--;
                }
                free(novoReg);
                metricas->trocas++;
            } else {

                memoria[indiceMenor].id = -1;
                registrosNaMemoria--;
            }
        }
        fclose(arqParticao);


        registrosNaMemoria = posRes;
        for (int i = 0; i < posRes; i++) {
            memoria[i] = reservatorio[i];
        }
        posRes = 0;
    }

    free(memoria);
    free(reservatorio);
    return numParticoes;
}
