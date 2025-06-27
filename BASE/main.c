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

    //-----------  Cria as bases de dados (caso vá utilizar, se lembre de mudar a leitura para wb+)  -----------
    criarBaseMercado(200, 5000, 60, forn, prod, caix);
    //                ^^^^^^^^^^^^^^^^ > Tamanho padrão da base
    //quicksortFornec(forn, 200, log);
    //quicksortProd(prod, 5000, log);

    //-----------  Imprime especifico (falta implementar)  -----------
    //consultarFornecedorPorId(forn, 100, log);
    //consultarProdutoPorId(prod, 500, log);
    //consultarCaixaPorId(caix, 30, log);

    //-----------  Imprime as bases  -----------
    //imprimirBaseFornec(forn);
    //imprimirBaseProd(prod);
    //imprimirBaseCaixa(caix);

    //-----------  Cadastra  -----------
    //cadastrarFornecedor(forn);
    //cadastrarProduto(prod, forn);
    //adicionarCaixas(3, caix);

    //-----------  Operações  -----------
    //LEMBRE DE DESCOMENTAR O "quicksortProd"
    //Vender(30, 500, caix, prod, log);
    //atualizarProduto(prod, forn, log);
    //imprimirEstoqueBaixo(prod, 5, log);

    //-----------  Imprime as bases  -----------
    //imprimirBaseFornec(forn);
    //imprimirBaseProd(prod);
    //imprimirBaseCaixa(caix);

    //-----------  Imprime especifico (falta implementar) -----------
    //consultarFornecedorPorId(forn, 100, log);
    //consultarProdutoPorId(prod, 500, log);
    //consultarCaixaPorId(caix, 30, log);

    //-----------  Fecha os arquivos  -----------
    fclose(forn);
    fclose(prod);
    fclose(caix);
    fclose(log);

    printf("\n\nPrograma finalizado com sucesso.\n");
    return 0;
}
