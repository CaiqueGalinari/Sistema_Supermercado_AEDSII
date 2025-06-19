#include "funcionario.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

//Realiza uma busca sequencial por um funcionario na base de dados
TFunc *buscaSequencial(int chave, FILE *in, FILE *log){

    TFunc *f;
    int achou;
    int cont = 0;
    clock_t inicio, fim;
    double total;

    rewind(in);

    inicio = clock();

    while ((f = le(in)) != NULL){

        cont ++;

        if(f->cod == chave){
           achou = 1;
           break;
        }
    }
        if(achou == 1){
            fprintf(log, "\nComparacoes Sequencial: %d ", cont);
            fim = clock();
            total = (fim - inicio)/CLOCKS_PER_SEC;
            fprintf(log, "\nTempo Sequencial: %f ", total);
            return f;
        }

        else {
            printf("Funcionario nao encontrado");
            fprintf(log, "Comparacoes Sequencial: %d ", cont);
            fim = clock();
            total = (fim - inicio)/CLOCKS_PER_SEC;
            fprintf(log, "Tempo Sequencial: %f ", cont);
        }

        free(f);
}
