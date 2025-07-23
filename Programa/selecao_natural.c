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

    // Carregar mem�ria inicial com os primeiros M registros
    for (posMem = 0; posMem < M; posMem++) {
        TFornecedor *f = leFornec(arqEntrada);
        if (f) {
            memoria[posMem] = *f;
            free(f);
            metricas->trocas++; // Contabiliza a leitura do arquivo para a mem�ria
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

    // Loop principal: continua enquanto houver registros a serem processados na mem�ria
    while (registrosNaMemoria > 0) {
        char nomeParticao[256];
        sprintf(nomeParticao, "%s%d.dat", nomeBase, numParticoes);
        FILE* arqParticao = fopen(nomeParticao, "wb");
        numParticoes++;

        int idUltimoEscrito = -1; // ID do �ltimo registro salvo na parti��o atual

        // Loop da parti��o: continua enquanto houver registros na mem�ria que possam ser adicionados � parti��o
        while (registrosNaMemoria > 0) {
            int menorId = INT_MAX;
            int indiceMenor = -1;

            // Encontra o menor registro na mem�ria que pode continuar a sequ�ncia ordenada
            for (int i = 0; i < M; i++) {
                if(memoria[i].id != -1){ // Posi��o n�o est� "congelada"
                    metricas->comparacoes++;
                    if (memoria[i].id >= idUltimoEscrito && memoria[i].id < menorId) {
                        menorId = memoria[i].id;
                        indiceMenor = i;
                    }
                }
            }

            // Se n�o encontrou nenhum, a parti��o atual terminou
            if (indiceMenor == -1) {
                break;
            }

            // Salva o menor registro encontrado na parti��o
            TFornecedor menorFornecedor = memoria[indiceMenor];
            salvaFornec(&menorFornecedor, arqParticao);
            metricas->trocas++; // Contabiliza a escrita da mem�ria para o disco
            idUltimoEscrito = menorFornecedor.id;

            // Substitui o registro salvo por um novo do arquivo de entrada
            TFornecedor* novoReg = leFornec(arqEntrada);
            if (novoReg) {
                metricas->comparacoes++;
                if (novoReg->id >= idUltimoEscrito) {
                    // O novo registro pertence � parti��o atual
                    memoria[indiceMenor] = *novoReg;
                } else {
                    // O novo registro pertence � pr�xima parti��o, vai para o reservat�rio
                    reservatorio[posRes++] = *novoReg;
                    memoria[indiceMenor].id = -1; // "Congela" a posi��o na mem�ria
                    registrosNaMemoria--;
                }
                free(novoReg);
                metricas->trocas++; // Contabiliza a leitura para mem�ria/reservat�rio
            } else {
                // N�o h� mais registros no arquivo de entrada, apenas "congela" a posi��o
                memoria[indiceMenor].id = -1;
                registrosNaMemoria--;
            }
        }
        fclose(arqParticao);

        // Prepara para a pr�xima parti��o: move os registros do reservat�rio para a mem�ria
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
