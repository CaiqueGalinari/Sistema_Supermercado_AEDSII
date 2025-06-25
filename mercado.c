#include "mercado.h"
#include <time.h>

void criarBaseMercado(int num_fornecedores, int num_produtos, int num_caixas, FILE *out_fornecedores, FILE *out_produtos, FILE *out_caixas){
    if (!out_fornecedores || !out_produtos || !out_caixas) {
        printf("ERRO: Um ou mais ponteiros de arquivo sao nulos. A operacao foi cancelada.\n");
        return;
    }

    srand(time(NULL));
    printf("### INICIANDO GERACAO COMPLETA DA BASE DE DADOS ###\n");

    // ========================================================================
    // ETAPA 1: CRIA��O DE FORNECEDORES
    // ========================================================================
    printf("\nGerando a base de dados de Fornecedores...\n");
    char* prefixos[] = {"Distribuidora", "Comercio", "Importadora", "Atacado", "Central"};
    char* sufixos[] = {"LTDA", "e Cia", "S.A.", "Express", "do Brasil"};
    for(int i = 0; i < num_fornecedores; i++){
        char nome[100], cnpj[20], tel[15]; // N�meros substitu�dos aqui
        sprintf(nome, "%s de Suprimentos %s", prefixos[rand() % 5], sufixos[rand() % 5]);
        sprintf(cnpj, "%02d.%03d.%03d/0001-%02d", rand() % 99 + 1, rand() % 1000, rand() % 1000, rand() % 100);
        sprintf(tel, "319%08d", rand() % 100000000);
        TFornecedor *f = fornecedor(i + 1, nome, cnpj, tel);
        salvaFornec(f, out_fornecedores);
        free(f);
    }
    printf("Base de Fornecedores gerada com sucesso!\n");

    // ========================================================================
    // ETAPA 2: CRIA��O DE PRODUTOS
    // ========================================================================
    printf("\nGerando a base de dados de Produtos...\n");
    TProduto** produtos_em_memoria = (TProduto**) malloc(sizeof(TProduto*) * num_produtos);

    for(int i = 0; i < num_produtos; i++){
        char nome[100], desc[256]; // N�meros substitu�dos aqui
        sprintf(nome, "Produto %d", i + 1);
        sprintf(desc, "Descricao detalhada do Produto %d", i + 1);
        float preco_aleatorio = ((float)(rand() % 30000) / 100.0f) + 1.0f;
        int estoque_aleatorio = rand() % 500;
        int id_fornecedor_aleatorio = (rand() % num_fornecedores) + 1;

        TProduto *p = produto(i + 1, nome, desc, preco_aleatorio, estoque_aleatorio, id_fornecedor_aleatorio);
        salvaProd(p, out_produtos);
        produtos_em_memoria[i] = p;
    }
    printf("Base de Produtos gerada com sucesso!\n");

    // ========================================================================
    // ETAPA 3: CRIA��O DE CAIXAS (nenhuma constante para substituir aqui)
    // ========================================================================
    printf("\nGerando a base de dados de Caixas...\n");
    for (int i=0; i < num_caixas; i++){
        int num_produtos_vendidos = (rand() % 5) + 1;
        TListaprod* lista = (TListaprod*) malloc(sizeof(TListaprod) * num_produtos_vendidos);
        for(int j=0; j < num_produtos_vendidos; j++){
            int produto_idx = rand() % num_produtos;
            lista[j].produto = *produtos_em_memoria[produto_idx];

            int qtd_a_vender = (rand() % 10) + 1;
            if (lista[j].produto.estoque > 0 && qtd_a_vender > lista[j].produto.estoque) {
                qtd_a_vender = lista[j].produto.estoque;
            } else if (lista[j].produto.estoque == 0) {
                qtd_a_vender = 0;
            }
            lista[j].qtdvend = qtd_a_vender;
            lista[j].valortotal = lista[j].produto.preco * lista[j].qtdvend;
        }
        TCaixa *c = caixa(i + 1, lista, num_produtos_vendidos);
        salvaCaixa(c, out_caixas);
        free(lista);
        free(c);
    }
    printf("Base de Caixas gerada com sucesso!\n");

    // ========================================================================
    // ETAPA 4: LIMPEZA DA MEM�RIA
    // ========================================================================
    for (int i = 0; i < num_produtos; i++) {
        free(produtos_em_memoria[i]);
    }
    free(produtos_em_memoria);

    printf("\n### GERACAO DA BASE DE DADOS CONCLUIDA COM SUCESSO! ###\n");
}

void adicionarCaixas(int num_caixas, FILE *out_caixas){
    if (!out_caixas) {
        printf("ERRO: Ponteiro de arquivo para caixas � nulo. A opera��o foi cancelada.\n");
        return;
    }

    printf("\n### INICIANDO ADI��O DE CAIXAS ###\n");

    //Gera o id igual no resto
    int proximo_id = 1;
    TCaixa *caixa_temp;
    rewind(out_caixas);
    while ((caixa_temp = leCaixa(out_caixas)) != NULL) { //
        if (caixa_temp->id >= proximo_id) {
            proximo_id = caixa_temp->id + 1;
        }
        if (caixa_temp->lista) {
            free(caixa_temp->lista);
        }
        free(caixa_temp);
    }



    // Adiciona os caixas
    fseek(out_caixas, 0, SEEK_END); //Cursor no final

    for (int i = 0; i < num_caixas; i++){
        // Para cada novo caixa, a lista de produtos � nula e o n�mero de produtos � 0
        TCaixa *c = caixa(proximo_id + i, NULL, 0);
        if(c){
            salvaCaixa(c, out_caixas); //
            free(c);
        }
    }

    printf("%d caixas zerados foram adicionados com sucesso, come�ando pelo ID %d.\n", num_caixas, proximo_id);
    printf("### FINALIZADO ###\n");
}
