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
        FILE *prod_hash = fopen("produtos.hash", "wb+");
    FILE *caix = fopen("caixas.dat", "wb+");
    FILE *log = fopen("log.txt", "w+");
    #define NUM_FORNEC 15
    #define NUM_PROD 500
    #define NUM_CAIX 10


    if (forn == NULL || prod == NULL || caix == NULL || prod_hash == NULL) {
        printf("Erro ao abrir os arquivos \n");
        return 1;
    }
    int menu = 0, escolha = 0, escolha2 = 0 ;


    criarBaseMercado(NUM_FORNEC, NUM_PROD, NUM_CAIX, forn, prod, caix, prod_hash);


    do{
    //------- menu ---------
    printf("\n******************************\n");
    printf("O que voce deseja fazer?\n");
    printf("1 - Imprimir fornecedores\n");
    printf("2 - Imprimir produtos\n");
    printf("3 - Imprimir Caixas\n");
    printf("4 - Ordenar fornecedores por particao\n");
    printf("5 - Procurar fornecedor por ID\n");
    printf("6 - Procurar produto por ID (hash) \n"); //hash
    printf("7 - Procurar caixa por ID\n");
    printf("8 - Cadastrar fornecedor\n");
    printf("9 - Cadastrar produto (Hash)\n");
    printf("10 - Adicionar caixas\n");
    printf("11 - Vender \n");
    printf("12 - Atualizar dados de um produto (hash) \n"); // hash
    printf("13 - Conferir estoque baixo\n");
    printf("14 - Remover produto por ID (Hash)\n"); // hash
    printf("0 - sair\n");
    scanf("%d",&menu);


switch(menu){
        case 1:
            imprimirBaseFornec(forn);
            break;
        case 2:
            imprimirBaseProd(prod, prod_hash);
            break;
        case 3:
            imprimirBaseCaixa(caix);
            break;
        case 4:
            printf("Ordenando fornecedores...\n");
            // Ajuste o número de partições conforme a memória disponível
            OrdenacaoEmDiscoFornec(forn, forn, log, 1000);
            printf("Fornecedores ordenados.\n");
            break;
        case 5:
            printf("Qual o ID do fornecedor desejado?\n");
            scanf("%d",&escolha);
            consultarFornecedorPorId(forn, escolha, log);
            break;
        case 6:
            printf("Qual o ID do produto desejado?\n");
            scanf("%d",&escolha);
            consultarProdutoPorIdHash(prod, prod_hash, escolha, log);
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
            cadastrarProduto(prod, prod_hash, forn);
            break;
        case 10:
            printf("Quantos caixas a serem adicionados?\n");
            scanf("%d",&escolha);
            adicionarCaixas(caix,escolha);
            break;
        case 11:
            printf("Qual o ID do caixa que fez a venda?\n");
            scanf("%d",&escolha);
            printf("Qual o ID do produto vendido?\n");
            scanf("%d",&escolha2);
            Vender(escolha, escolha2, caix, prod, prod_hash, log);
            break;
        case 12:
            atualizarProduto(prod, forn, prod_hash, log);
            break;
        case 13:
            imprimirEstoqueBaixo(prod, 5, log);
            break;
        case 14:
            printf("Qual o ID do produto a ser removido?\n");
            scanf("%d", &escolha);
            removeProdHash(escolha, prod, prod_hash);
            break;
        case 0:
            // Sair
            break;
        default:
            printf("Opcao invalida! Tente novamente.\n");
            break;
    }

    escolha = 0;
    escolha2 = 0;

    }while(menu != 0);
    //-----------  Fecha os arquivos  -----------
    fclose(forn);
    fclose(prod);
        fclose(prod_hash);
    fclose(caix);
    fclose(log);

    printf("\n\nPrograma finalizado com sucesso.\n");
    return 0;
}
