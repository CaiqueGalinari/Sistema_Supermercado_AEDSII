#include <stdio.h>
#include <stdlib.h>
#include "mercado.h"
#include "fornecedor.h"
#include "produto.h"
#include "caixa.h"


int main() {
    //-----------  Abre os arquivos em modo "wb+". TROQUE PARA "rb+" SE JÁ TIVER CRIADO A BASE (base criada por padrão) -----------
    FILE *forn = fopen("fornecedores.dat", "wb+");
    FILE *prod = fopen("produtos.dat", "wb+");
    FILE *caix = fopen("caixas.dat", "wb+");
    FILE *log = fopen("log.txt", "w+");
    #define NUM_FORNEC 15000
    #define NUM_PROD 1000
    #define NUM_CAIX 10


    if (forn == NULL || prod == NULL || caix == NULL) {
        printf("Erro ao abrir os arquivos \n");
        return 1;
    }
    int menu = 0, escolha = 0, escolha2 = 0 ;


    criarBaseMercado(NUM_FORNEC, NUM_PROD, NUM_CAIX, forn, prod, caix);


    do{
    //------- menu ---------
    printf("\n******************************\n");
    printf("O que voce deseja fazer?\n");
    printf("1 - Imprimir fornecedores\n");
    printf("2 - Imprimir produtos\n");
    printf("3 - Imprimir Caixas\n");
    printf("4 - Ordenar fornecedores (isto e necessario outras operacoes)\n");
    printf("5 - Ordenar fornecedores por partição (isto e necessario outras operacoes)\n");
    printf("6 - Ordenar produtos (isto e necessario outras operacoes)\n");
    printf("7 - Ordenar caixa\n");
    printf("8 - Procurar fornecedor por ID\n");
    printf("9 - Procurar produto por ID\n");
    printf("10 - Procurar caixa por ID\n");
    printf("11 - Cadastrar fornecedor\n");
    printf("12 - Cadastrar produto\n");
    printf("13 - Adicionar caixas\n");
    printf("14 - Vender \n");
    printf("15 - Atualizar dados de um produto\n");
    printf("16 - Conferir estoque baixo\n");
    printf("0 - sair\n");
    scanf("%d",&menu);


    switch(menu){
        case 1:
            imprimirBaseFornec(forn);
        break;
        case 2:
            imprimirBaseProd(prod);
        break;
        case 3:
            imprimirBaseCaixa(caix);
        break;
        case 4:
            quicksortFornec(forn, NUM_FORNEC, log);
            printf("Fornecedores ordenados\n");
        break;
        case 5:
            if(NUM_FORNEC <= 10000){
            OrdenacaoEmDiscoFornec(forn, forn, log, 100);
            } else{
            OrdenacaoEmDiscoFornec(forn, forn, log, 1000);
            }
        break;
        case 6:
            quicksortProd(prod, NUM_PROD, log);
            printf("Produtos ordenados\n");
        break;
        case 7:
            quicksortCaix(prod, NUM_CAIX, log);
            printf("Caixas ordenados\n");
        break;
        case 8:
            printf("Qual o ID do fornecedor desejado?\n");
            scanf("%d",&escolha);
            consultarFornecedorPorId(forn, escolha, log);
        break;
        case 9:
            printf("Qual o ID do produto desejado?\n");
            scanf("%d",&escolha);
           consultarProdutoPorId(prod, escolha, log);
        break;
        case 10:
            printf("Qual o ID do caixa desejado?\n");
            scanf("%d",&escolha);
            consultarCaixaPorId(caix, escolha, log);
        break;
        case 11:
            cadastrarFornecedor(forn);
        break;
        case 12:
            cadastrarProduto(prod, forn);
        break;
        case 13:
            printf("Quantos caixas a serem adionados\n");
            scanf("%d",&escolha);
            adicionarCaixas(caix,escolha);
        break;
        case 14:
            printf("Qual o ID do caixa que fez a venda?\n");
            scanf("%d",&escolha);
            printf("Qual o ID do produto vendido?\n");
            scanf("%d",&escolha2);
            Vender(escolha, escolha2, caix, prod, log);
        break;
        case 15:
            atualizarProduto(prod, forn, log);
        break;
        case 16:
            imprimirEstoqueBaixo(prod, 5, log);
        break;
        default:
        break;
    }

    escolha = 0;
    escolha2 = 0;

    }while(menu != 0);
    //-----------  Fecha os arquivos  -----------
    fclose(forn);
    fclose(prod);
    fclose(caix);
    fclose(log);

    printf("\n\nPrograma finalizado com sucesso.\n");
    return 0;
}
