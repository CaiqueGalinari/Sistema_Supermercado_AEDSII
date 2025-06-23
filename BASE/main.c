#include <stdio.h>
#include <stdlib.h>
#include "mercado.h"
#include "fornecedor.h"
#include "produto.h"
#include "caixa.h"


int main() {
    // Abre os arquivos em modo "wb+" que apaga o conteúdo anterior, cria se não existir, e permite escrita e leitura.
    FILE *forn = fopen("fornecedores.dat", "wb+");
    FILE *prod = fopen("produtos.dat", "wb+");
    FILE *caix = fopen("caixas.dat", "wb+");

    if (forn == NULL || prod == NULL || caix == NULL) {
        printf("Erro ao abrir os arquivos .dat\n");
        return 1;
    }

    // Cria as bases de dados
    criarBaseMercado(20, 50, 5, forn, prod, caix);

    // Imprime as bases de dados para verificação
    imprimirBaseFornec(forn);
    imprimirBaseProd(prod);
    imprimirBaseCaixa(caix);

    // Fecha os arquivos
    fclose(forn);
    fclose(prod);
    fclose(caix);

    printf("\n\nPrograma finalizado com sucesso.\n");
    return 0;
}
