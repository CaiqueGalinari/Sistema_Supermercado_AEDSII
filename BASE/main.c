#include <stdio.h>
#include <stdlib.h>
#include "mercado.h"
#include "fornecedor.h"
#include "produto.h"
#include "caixa.h"


int main() {
    //-----------  Abre os arquivos em modo "wb+". TROQUE PARA "rb+" SE JÁ TIVER CRIADO A BASE (base criada por padrão) -----------
    FILE *forn = fopen("fornecedores.dat", "rb+");
    FILE *prod = fopen("produtos.dat", "rb+");
    FILE *caix = fopen("caixas.dat", "rb+");
    FILE *log = fopen("log.txt", "w+");

    if (forn == NULL || prod == NULL || caix == NULL) {
        printf("Erro ao abrir os arquivos \n");
        return 1;
    }

    //-----------  Cria as bases de dados (caso vá utilizar, se lembre de mudar a leitura para wb+)  -----------
    //criarBaseMercado(200, 5000, 60, forn, prod, caix);
    //                ^^^^^^^^^^^^^^^^ > Tamanho padrão da base

    //-----------  Imprime especifico (falta implementar)  -----------


    //-----------  Imprime as bases  -----------
    //imprimirBaseFornec(forn);
    //imprimirBaseProd(prod);
    //imprimirBaseCaixa(caix);

    //-----------  Cadastra  -----------
    //cadastrarProduto(prod, forn);
    //cadastrarFornecedor(forn);
    //adicionarCaixas(3, caix);

    //-----------  Operações  -----------
    //Vender(30, 500, caix, prod, log);

    //-----------  Imprime as bases  -----------
    //imprimirBaseFornec(forn);
    //imprimirBaseProd(prod);
    //imprimirBaseCaixa(caix);

    //-----------  Imprime especifico (falta implementar) -----------

    //-----------  Fecha os arquivos  -----------
    fclose(forn);
    fclose(prod);
    fclose(caix);
    fclose(log);

    printf("\n\nPrograma finalizado com sucesso.\n");
    return 0;
}
