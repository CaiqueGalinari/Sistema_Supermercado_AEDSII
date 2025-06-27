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
    
    if (forn == NULL || prod == NULL || caix == NULL) {
        printf("Erro ao abrir os arquivos \n");
        return 1;
    }
    int menu = 0, escolha = 0, escolha2 = 0 ;
    //-----------  Cria as bases de dados (caso vá utilizar, se lembre de mudar a leitura para wb+)  -----------
    criarBaseMercado(200, 5000, 60, forn, prod, caix);
    //                ^^^^^^^^^^^^^^^^ > Tamanho padrão da base

    do{
    //------- menu ---------
    printf("******************************\n");
    printf("O que voce deseja fazer?\n");
    printf("1 - Imprimir fornecedores\n");
    printf("2 - Imprimir produtos\n");
    printf("3 - Imprimir Caixas\n");
    printf("4 - Ordenar dados (isto é necessario para outras operaçoes)\n");
    printf("5 - Procurar fornecedor por ID\n");
    printf("6 - Procurar produto por ID\n");
    printf("7 - Procurar caixa por ID\n");
    printf("8 - Cadastrar fornecedor\n");
    printf("9 - Cadastrar produto\n");
    printf("10 - Adicionar caixas\n");
    printf("11 - Vender \n");
    printf("12 - Atualizar dados de um produto\n");
    printf("13 - Conferir Estoque\n");
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
        //------- ordena --------
            quicksortFornec(forn, 200, log);
            quicksortProd(prod, 5000, log);
            quicksortCaix(caix, 60, log);
            printf("Dados ordenados\n");
        break;
        case 5:
            printf("Qual o ID do fornecedor desejado?\n");
            scanf("%d",&escolha);
            consultarFornecedorPorId(forn, escolha, log);
        break;
        case 6:
            printf("Qual o ID do produto desejado?\n");
            scanf("%d",&escolha);
           consultarProdutoPorId(prod, escolha, log);
        break;
        case 7:
            printf("Qual o ID do caixa desejado?\n");
            scanf("%d",&escolha);
            consultarCaixaPorId(caix, escolha, log);
        break;
        case 8:
            cadastrarFornecedor(forn);
        break;
        case 9:
            cadastrarProduto(prod, forn);
        break;
        case 10:
            printf("Quantos caixas a serem adionados\n");
            scanf("%d",&escolha);
            adicionarCaixas(caix,escolha);
        break;
        case 11:
            printf("Qual o ID do caixa que fez a venda?\n");
            scanf("%d",&escolha);
            printf("Qual o ID do produto vendido?\n");
            scanf("%d",&escolha2);
            Vender(30, 500, caix, prod, log);
        break;
        case 12:
            atualizarProduto(prod, forn, log);
        break;
        case 13:
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
