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

    // Carregar memória inicial com os primeiros M registros
    for (posMem = 0; posMem < M; posMem++) {
        TFornecedor *f = leFornec(arqEntrada);
        if (f) {
            memoria[posMem] = *f;
            free(f);
            metricas->trocas++; // Contabiliza a leitura do arquivo para a memória
        } else {
            break; // Fim do arquivo de entrada
        }
    }

    if(posMem == 0){ // Caso o arquivo de entrada esteja vazio
        free(memoria);
        free(reservatorio);
        return 0;
    }

    int registrosNaMemoria = posMem;

    // Loop principal: continua enquanto houver registros a serem processados na memória
    while (registrosNaMemoria > 0) {
        char nomeParticao[256];
        sprintf(nomeParticao, "%s%d.dat", nomeBase, numParticoes);
        FILE* arqParticao = fopen(nomeParticao, "wb");
        numParticoes++;

        int idUltimoEscrito = -1; // ID do último registro salvo na partição atual

        // Loop da partição: continua enquanto houver registros na memória que possam ser adicionados à partição
        while (registrosNaMemoria > 0) {
            int menorId = INT_MAX;
            int indiceMenor = -1;

            // Encontra o menor registro na memória que pode continuar a sequência ordenada
            for (int i = 0; i < M; i++) {
                if(memoria[i].id != -1){ // Posição não está "congelada"
                    metricas->comparacoes++;
                    if (memoria[i].id >= idUltimoEscrito && memoria[i].id < menorId) {
                        menorId = memoria[i].id;
                        indiceMenor = i;
                    }
                }
            }

            // Se não encontrou nenhum, a partição atual terminou
            if (indiceMenor == -1) {
                break;
            }

            // Salva o menor registro encontrado na partição
            TFornecedor menorFornecedor = memoria[indiceMenor];
            salvaFornec(&menorFornecedor, arqParticao);
            metricas->trocas++; // Contabiliza a escrita da memória para o disco
            idUltimoEscrito = menorFornecedor.id;

            // Substitui o registro salvo por um novo do arquivo de entrada
            TFornecedor* novoReg = leFornec(arqEntrada);
            if (novoReg) {
                metricas->comparacoes++;
                if (novoReg->id >= idUltimoEscrito) {
                    // O novo registro pertence à partição atual
                    memoria[indiceMenor] = *novoReg;
                } else {
                    // O novo registro pertence à próxima partição, vai para o reservatório
                    reservatorio[posRes++] = *novoReg;
                    memoria[indiceMenor].id = -1; // "Congela" a posição na memória
                    registrosNaMemoria--;
                }
                free(novoReg);
                metricas->trocas++; // Contabiliza a leitura para memória/reservatório
            } else {
                // Não há mais registros no arquivo de entrada, apenas "congela" a posição
                memoria[indiceMenor].id = -1;
                registrosNaMemoria--;
            }
        }
        fclose(arqParticao);

        // Prepara para a próxima partição: move os registros do reservatório para a memória
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
