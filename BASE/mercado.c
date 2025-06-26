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
    // ETAPA 1: CRIAÇÃO DE FORNECEDORES
    // ========================================================================
    printf("\nGerando a base de dados de Fornecedores...\n");
    char* prefixos[] = {"Distribuidora", "Comercio", "Importadora", "Atacado", "Central"};
    char* sufixos[] = {"LTDA", "e Cia", "S.A.", "Express", "do Brasil"};
    for(int i = 0; i < num_fornecedores; i++){
        char nome[100], cnpj[20], tel[15]; // Números substituídos aqui
        sprintf(nome, "%s de Suprimentos %s", prefixos[rand() % 5], sufixos[rand() % 5]);
        sprintf(cnpj, "%02d.%03d.%03d/0001-%02d", rand() % 99 + 1, rand() % 1000, rand() % 1000, rand() % 100);
        sprintf(tel, "319%08d", rand() % 100000000);
        TFornecedor *f = fornecedor(i + 1, nome, cnpj, tel);
        salvaFornec(f, out_fornecedores);
        free(f);
    }
    printf("Base de Fornecedores gerada com sucesso!\n");

    // ========================================================================
    // ETAPA 2: CRIAÇÃO DE PRODUTOS
    // ========================================================================
    printf("\nGerando a base de dados de Produtos...\n");
    TProduto** produtos_em_memoria = (TProduto**) malloc(sizeof(TProduto*) * num_produtos);

    for(int i = 0; i < num_produtos; i++){
        char nome[100], desc[256]; // Números substituídos aqui
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
    // ETAPA 3: CRIAÇÃO DE CAIXAS (nenhuma constante para substituir aqui)
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
    // ETAPA 4: LIMPEZA DA MEMÓRIA
    // ========================================================================
    for (int i = 0; i < num_produtos; i++) {
        free(produtos_em_memoria[i]);
    }
    free(produtos_em_memoria);

    printf("\n### GERACAO DA BASE DE DADOS CONCLUIDA COM SUCESSO! ###\n");
}

void adicionarCaixas(int num_caixas, FILE *out_caixas){
    if (!out_caixas) {
        printf("ERRO: Ponteiro de arquivo para caixas é nulo. A operação foi cancelada.\n");
        return;
    }

    printf("\n### INICIANDO ADIÇÃO DE CAIXAS ###\n");

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
        // Para cada novo caixa, a lista de produtos é nula e o número de produtos é 0
        TCaixa *c = caixa(proximo_id + i, NULL, 0);
        if(c){
            salvaCaixa(c, out_caixas); //
            free(c);
        }
    }

    printf("%d caixas zerados foram adicionados com sucesso, começando pelo ID %d.\n", num_caixas, proximo_id);
    printf("### FINALIZADO ###\n");
}

//Funções de busca binaria e sequencial praticamente iguais da aula, com alterações para caber no código
TProduto *buscaBinariaProduto(int chave, FILE *in, int inicio, int fim, long *pos_encontrada, FILE *log) {
    TProduto *p = NULL;
    int cod = -1;
    int cont = 0;
    clock_t inicioT, fimT;
    double total;

    inicioT = clock();

    while (inicio <= fim) {
        cont++;
        int meio = trunc((inicio + fim) / 2.0);

        *pos_encontrada = (long)(meio - 1) * sizeof(TProduto);
        fseek(in, *pos_encontrada, SEEK_SET);
        p = leProd(in);

        if (!p) break;

        cod = p->id;

        if (cod == chave) {
            fprintf(log, "\n-- Busca Binária Produto (ID: %d) --", chave);
            fprintf(log, "\nComparacoes: %d", cont);
            fimT = clock();
            total = (double)(fimT - inicioT) / CLOCKS_PER_SEC;
            fprintf(log, "\nTempo: %f segundos\n", total);
            return p;
        }

        if (cod > chave) {
            fim = meio - 1;
        } else {
            inicio = meio + 1;
        }
        free(p);
    }

    *pos_encontrada = -1;
    return NULL;
}

TCaixa *buscaSequencialCaixa(int chave, FILE *in, long *pos_encontrada, FILE *log) {
    TCaixa *c;
    int achou = 0;
    int cont = 0;
    clock_t inicio, fim;
    double total;

    rewind(in);
    inicio = clock();

    while (1) {
        *pos_encontrada = ftell(in);
        c = leCaixa(in);
        if (c == NULL) {
            break;
        }

        cont++;

        if (c->id == chave) {
           achou = 1;
           break;
        }

        if (c->lista) free(c->lista);
        free(c);
    }

    fim = clock();
    total = (double)(fim - inicio) / CLOCKS_PER_SEC;

    if (achou == 1) {
        fprintf(log, "\n-- Busca Sequencial Caixa (ID: %d) --", chave);
        fprintf(log, "\nComparacoes: %d", cont);
        fprintf(log, "\nTempo: %f segundos\n", total);
        return c;
    } else {
        printf("Caixa nao encontrado\n");
        fprintf(log, "\n-- Busca Sequencial Caixa (ID: %d) --", chave);
        fprintf(log, "\nNao encontrado. Comparacoes: %d", cont);
        fprintf(log, "\nTempo: %f segundos\n", total);
        *pos_encontrada = -1;
        return NULL;
    }
}



void Vender(int id_caixa, int id_produto, FILE* arq_caixas, FILE* arq_produtos, FILE* log) {
    //Acha o produto
    long pos_produto = -1;
    fseek(arq_produtos, 0, SEEK_END);
    int num_produtos = ftell(arq_produtos) / sizeof(TProduto); //Pega o numero de produtos

    TProduto *produto_vendido = buscaBinariaProduto(id_produto, arq_produtos, 1, num_produtos, &pos_produto, log); //busca binaria

    //Se não achar...
    if (produto_vendido == NULL) {
        printf("ERRO: Produto com ID %d não encontrado!\n", id_produto);
        fprintf(log, "\nERRO: Produto com ID %d não encontrado na busca.\n", id_produto);
        return;
    }

    //Se não tiver em estoque...
    if (produto_vendido->estoque <= 0) {
        printf("ERRO: Produto '%s' (ID: %d) sem estoque!\n", produto_vendido->nome, id_produto);
        free(produto_vendido);
        return;
    }




    //Acha o caixa
    long pos_caixa = -1;
    TCaixa *caixa_venda = buscaSequencialCaixa(id_caixa, arq_caixas, &pos_caixa, log);

    if (caixa_venda == NULL) {
        // A própria buscaSequencialCaixa já imprime "Caixa nao encontrado"
        fprintf(log, "\nERRO: Caixa com ID %d não encontrado na busca.\n", id_caixa);
        free(produto_vendido);
        return;
    }




    //Processa a venda
    //Verifica se o produto já ta na lista
    int produto_ja_na_lista = 0;
    for (int i = 0; i < caixa_venda->num_produtos; i++) {
        //Se tiver...
        if (caixa_venda->lista[i].produto.id == id_produto) {
            caixa_venda->lista[i].qtdvend++; //aumenta a quantidade vendida
            caixa_venda->lista[i].valortotal = caixa_venda->lista[i].qtdvend * caixa_venda->lista[i].produto.preco; //atualiza o valor total
            produto_ja_na_lista = 1;
            break;
        }
    }

    //Se não tiver, ele adiciona na lista e faz a mesma coisa
    if (!produto_ja_na_lista) {
        caixa_venda->num_produtos++;
        caixa_venda->lista = (TListaprod *) realloc(caixa_venda->lista, caixa_venda->num_produtos * sizeof(TListaprod));
        if (caixa_venda->lista == NULL) {
             printf("ERRO: Falha ao alocar memória para a lista de produtos do caixa!\n");
             free(produto_vendido);
             if (caixa_venda->lista) free(caixa_venda->lista);
             free(caixa_venda);
             return;
        }
        TListaprod *novo_item = &caixa_venda->lista[caixa_venda->num_produtos - 1];
        novo_item->produto = *produto_vendido;
        novo_item->qtdvend = 1;
        novo_item->valortotal = novo_item->produto.preco;
    }

    //Atualiza o estoque e salva as alterações
    produto_vendido->estoque--;
    fseek(arq_produtos, pos_produto, SEEK_SET);
    salvaProd(produto_vendido, arq_produtos); //Salva na posição que ele estava, dando overwrite

    fseek(arq_caixas, pos_caixa, SEEK_SET);
    salvaCaixa(caixa_venda, arq_caixas); //Mesma coisa aqui

    printf("Venda realizada com sucesso!\n -> Produto: %s | Novo Estoque: %d\n -> Caixa: %d | Total de itens na compra: %d\n",
           produto_vendido->nome, produto_vendido->estoque, id_caixa, caixa_venda->num_produtos);

    //Limpa a memoria
    free(produto_vendido);
    if(caixa_venda->lista) free(caixa_venda->lista);
    free(caixa_venda);
}
