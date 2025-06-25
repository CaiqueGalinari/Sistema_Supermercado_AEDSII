#include <stdio.h>
#include <stdlib.h>
#include "mercado.h"
#include "fornecedor.h"
#include "produto.h"
#include "caixa.h"


int main() {
    //-----------  Abre os arquivos em modo "wb+". TROQUE PARA "rb+" SE NÃO FOR USAR O criarBaseMercado NOVAMENTE  -----------
    FILE *forn = fopen("fornecedores.dat", "rb+");
    FILE *prod = fopen("produtos.dat", "rb+");
    FILE *caix = fopen("caixas.dat", "rb+");

    if (forn == NULL || prod == NULL || caix == NULL) {
        printf("Erro ao abrir os arquivos \n");
        return 1;
    }

    //-----------  Cria as bases de dados  -----------
    //criarBaseMercado(4, 5, 3, forn, prod, caix);

    imprimirBaseFornec(forn);
    //imprimirBaseProd(prod);
    //imprimirBaseCaixa(caix);

    //-----------  Cadastra  -----------
    //cadastrarProduto(prod, forn);
    //cadastrarFornecedor(forn);
    //adicionarCaixas(3, caix);

    // Imprime as bases de dados para verificação
    //imprimirBaseFornec(forn);
    //imprimirBaseProd(prod);
    //imprimirBaseCaixa(caix);

    //-----------  Fecha os arquivos  -----------
    fclose(forn);
    fclose(prod);
    fclose(caix);

    printf("\n\nPrograma finalizado com sucesso.\n");
    return 0;
}
