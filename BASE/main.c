#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funcionario.h"
#include "buscaSequencial.h"
#include "buscaBinaria.h"
#include "insertionSort.h"
#include "classificacaoInterna.h"
//#include "classificacaoInterna.h"
//#include "intercalacaoBasica.h"
//#include "metadados.c"




int main()
{
    FILE *arq;
    FILE *out;
    FILE *log;

    TFunc *f;
    int qtdparticoes;

    //TMetadados metadados;
    //metadados.topo_vagos = -1;

    /*if ((out = fopen("intercalado.dat", "w+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }*/

    if ((log = fopen("log.txt", "a+")) == NULL) {
            printf("Erro ao abrir arquivo\n");
            exit(1);
        }

    if ((arq = fopen("funcionario.dat", "w+b")) == NULL) {
        printf("Erro ao abrir arquivo\n");
        exit(1);
    }

    else{

        criarBase(arq, 100);
        //imprimirBase(arq);
        //insertionSort(arq,tamanho_arquivo(arq));
        classificacao_interna(arq, 20);
        FILE *p1 = fopen("partition0.dat", "rb");
        FILE *p2 = fopen("partition1.dat", "rb");
        FILE *p3 = fopen("partition2.dat", "rb");
        FILE *p4 = fopen("partition3.dat", "rb");
        FILE *p5 = fopen("partition4.dat", "rb");
        //FILE *p6 = fopen("partition5.dat", "rb");
        //FILE *p7 = fopen("partition6.dat", "rb");
        //FILE *p8 = fopen("partition7.dat", "rb");
        //FILE *p9 = fopen("partition8.dat", "rb");
        //FILE *p10 = fopen("partition9.dat", "rb");
        //imprimirBase(arq);
        //printf("\n\n\nORDENANDO ORDENANDO ORDENANDO ORDENANDO ORDENANDO ORDENANDO ORDENANDO\n\n\n");
        //system("pause");
        //f = buscaSequencial(450000, arq, log);
        //imprime(f);
        //f = busca_binaria(450000, arq, 0, tamanho_arquivo(arq)-1,log);
        //imprime(f);
        //free(f);
        //insertionSort(arq,tamanho_arquivo(arq));
        //imprimirBase(arq);
        qtdparticoes = classificacao_interna(arq, 20);
        printf("%d\n\n\n\n\n\n\n\n\n", qtdparticoes);
        imprimirBase(p1);
        imprimirBase(p2);
        imprimirBase(p3);
        imprimirBase(p4);
        imprimirBase(p5);
        //imprimirBase(p6);
        //imprimirBase(p7);
        //imprimirBase(p8);
        //imprimirBase(p9);
        //imprimirBase(p10);
        //intercalacao_basica(out, qtdparticoes);
        //imprimirBase(out);
    }

}
