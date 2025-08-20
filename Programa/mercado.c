#include "mercado.h"
#include <time.h>
#include <math.h>
#include <unistd.h>
#include "selecao_natural.h"
#include "intercalacao_otima.h"

int nCaixas = 0;

void embaralha(int *vet, int tam) {
    for (int i = tam - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = vet[i];
        vet[i] = vet[j];
        vet[j] = temp;
    }
}

void criarBaseMercado(int num_fornecedores, int num_produtos, int num_caixas, FILE* out_fornecedores, FILE* out_produtos, FILE* out_caixas, FILE* out_hash) {
    srand(time(NULL));
    printf("\nGerando a base de dados...\n");
    criaHashProd(out_hash);

    printf("Criando %d fornecedores...\n", num_fornecedores);
    int *fornecedor_ids = (int *) malloc(sizeof(int) * num_fornecedores);
    if (!fornecedor_ids) return;
    for (int i = 0; i < num_fornecedores; i++) {
        fornecedor_ids[i] = i + 1;
    }
    embaralha(fornecedor_ids, num_fornecedores);

    for (int i = 0; i < num_fornecedores; i++) {
        char nome[100], cnpj[20], telefone[15];
        sprintf(nome, "Fornecedor %d", fornecedor_ids[i]);
        sprintf(cnpj, "XX.XXX.XXX/0001-%02d", i % 100);
        sprintf(telefone, "(XX)XXXXX-%04d", i);
        TFornecedor *f = fornecedor(fornecedor_ids[i], nome, cnpj, telefone);
        salvaFornec(f, out_fornecedores);
        free(f);
    }
    printf("Fornecedores criados.\n");

    printf("Criando %d produtos...\n", num_produtos);
    int *produto_ids = (int *) malloc(sizeof(int) * num_produtos);
    if (!produto_ids) { free(fornecedor_ids); return; }
    for (int i = 0; i < num_produtos; i++) {
        produto_ids[i] = i + 1;
    }
    embaralha(produto_ids, num_produtos);

    for (int i = 0; i < num_produtos; i++) {
        char nome[100], descricao[256];
        sprintf(nome, "Produto %d", produto_ids[i]);
        sprintf(descricao, "Descricao do produto %d", produto_ids[i]);
        float preco = (float)(rand() % 15000) / 100.0f;
        int estoque = rand() % 500;
        int id_fornecedor_aleatorio = fornecedor_ids[rand() % num_fornecedores];
        TProduto *p = produto(produto_ids[i], nome, descricao, preco, estoque, id_fornecedor_aleatorio);
        insereProdHash(p, out_produtos, out_hash); // mudado para hash
        free(p);
    }
    printf("Produtos criados.\n");

    printf("Criando %d caixas...\n", num_caixas);
    int *caixa_ids = (int *) malloc(sizeof(int) * num_caixas);
    if (!caixa_ids) { free(fornecedor_ids); free(produto_ids); return; }
    for (int i = 0; i < num_caixas; i++) {
        caixa_ids[i] = i + 1;
    }
    embaralha(caixa_ids, num_caixas);

    for (int i = 0; i < num_caixas; i++) {
        TCaixa *c = caixa(caixa_ids[i], 0.0f);
        salvaCaixa(c, out_caixas);
        free(c);
    }
    printf("Caixas criados.\n");

    free(fornecedor_ids);
    free(produto_ids);
    free(caixa_ids);
    printf("Base de dados criada com sucesso!\n");
}

void adicionarCaixas(FILE* arq_caixas, int num_a_adicionar) {
    printf("\nAdicionando %d novos caixas...\n", num_a_adicionar);
    fseek(arq_caixas, 0, SEEK_END);
    long tamanho_arquivo = ftell(arq_caixas);
    nCaixas = tamanho_arquivo / sizeof(TCaixa);
    for (int i = 0; i < num_a_adicionar; i++) {
        int novo_id = nCaixas + 1;
        TCaixa *c = caixa(novo_id, 0.0f);
        salvaCaixa(c, arq_caixas);
        free(c);
        nCaixas++;
        printf("Caixa com ID %d adicionado. Total de caixas agora: %d\n", novo_id, nCaixas);
    }
    fflush(arq_caixas);
    printf("Adicao de caixas concluida.\n");
}

TFornecedor *buscaBinariaFornecedor(int chave, FILE *in, int inicio, int fim, long *pos_encontrada, FILE *log) {
    TFornecedor *f = NULL;
    int cod = -1, cont = 0;
    clock_t inicioT, fimT;
    double total;
    inicioT = clock();
    while (inicio <= fim) {
        cont++;
        int meio = trunc((inicio + fim) / 2.0);
        *pos_encontrada = (long)(meio - 1) * sizeof(TFornecedor);
        fseek(in, *pos_encontrada, SEEK_SET);
        f = leFornec(in);
        if (!f) break;
        cod = f->id;
        if (cod == chave) {
            fprintf(log, "\n-- Busca Binária Fornecedor (ID: %d) --", chave);
            fprintf(log, "\nComparacoes: %d", cont);
            fimT = clock();
            total = (double)(fimT - inicioT) / CLOCKS_PER_SEC;
            fprintf(log, "\nTempo: %f segundos\n", total);
            return f;
        }
        if (cod > chave) fim = meio - 1;
        else inicio = meio + 1;
        free(f);
    }
    *pos_encontrada = -1;
    return NULL;
}

/* Trocado busca binaria para busca em hash
TProduto *buscaBinariaProduto(int chave, FILE *in, int inicio, int fim, long *pos_encontrada, FILE *log) {
    TProduto *p = NULL;
    int cod = -1, cont = 0;
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
        if (cod > chave) fim = meio - 1;
        else inicio = meio + 1;
        free(p);
    }
    *pos_encontrada = -1;
    return NULL;
}
*/



TCaixa *buscaSequencialCaixa(int chave, FILE *in, long *pos_encontrada, FILE *log) {
    TCaixa *c;
    int achou = 0;
    rewind(in);
    while(1){
        *pos_encontrada = ftell(in);
        c = leCaixa(in);
        if(c == NULL) break;
        if (c->id == chave) {
            achou = 1;
            break;
        }
        free(c);
    }
    if (achou) return c;
    else {
        *pos_encontrada = -1;
        return NULL;
    }
}

void OrdenacaoEmDiscoFornec(FILE* arqEntrada, FILE* arqSaida, FILE* arqLog, int M) {
    TMetrica metricas = {0.0, 0, 0};
    clock_t inicio_total = clock();

    rewind(arqEntrada);
    int numParticoes = selecaoNaturalFornec(arqEntrada, "particao_temp_", M, &metricas);


    const char* nomeSaidaTemporario = "sorted_temp_output.dat";
    intercalacaoOtimaFornec("particao_temp_", numParticoes, nomeSaidaTemporario, &metricas);


    FILE* arqFinalOrdenado = fopen(nomeSaidaTemporario, "rb");
    if (arqFinalOrdenado) {
        TFornecedor* fornec;
        rewind(arqSaida);

        while ((fornec = leFornec(arqFinalOrdenado)) != NULL) {
            salvaFornec(fornec, arqSaida);
            free(fornec);
        }
        fclose(arqFinalOrdenado);
        remove(nomeSaidaTemporario);
    }


    clock_t fim_total = clock();
    metricas.texec = ((double)(fim_total - inicio_total)) / CLOCKS_PER_SEC;


    if (arqLog) {
        fprintf(arqLog, "\n--- Metricas Ordenacao (Modo Stream) ---\n");
        fprintf(arqLog, "Memoria (M): %d registros\n", M);
        fprintf(arqLog, "Particoes Geradas: %d\n", numParticoes);
        fprintf(arqLog, "Comparacoes: %d\n", metricas.comparacoes);
        fprintf(arqLog, "Movimentacoes: %d\n", metricas.trocas);
        fprintf(arqLog, "Tempo: %.4f segundos\n", metricas.texec);
        fflush(arqLog);
    }
}

//Trocado de busca binaria para busca em hash
void Vender(int id_caixa, int id_produto, FILE* arq_caixas, FILE* arq_produtos, FILE* arq_hash, FILE* log) {
    long pos_produto = -1;
    TProduto *produto_vendido = buscaProdHash(id_produto, arq_produtos, arq_hash, &pos_produto);

    if (produto_vendido == NULL || produto_vendido->estoque <= 0) {
        printf("ERRO: Produto com ID %d nao encontrado ou sem estoque!\n", id_produto);
        if(produto_vendido) free(produto_vendido);
        return;
    }

    long pos_caixa = -1;
    TCaixa *caixa_venda = buscaSequencialCaixa(id_caixa, arq_caixas, &pos_caixa, log);

    if (caixa_venda == NULL) {
        printf("ERRO: Caixa com ID %d nao encontrado!\n", id_caixa);
        free(produto_vendido);
        return;
    }

    caixa_venda->valor_arrecadado += produto_vendido->preco;
    produto_vendido->estoque--;

    fseek(arq_caixas, pos_caixa, SEEK_SET);
    salvaCaixa(caixa_venda, arq_caixas);
    fflush(arq_caixas);

    fseek(arq_produtos, pos_produto, SEEK_SET);
    salvaProd(produto_vendido, arq_produtos);
    fflush(arq_produtos);

    printf("\nVenda realizada com sucesso!\n");
    printf(" -> Caixa %d | Novo valor arrecadado: R$%.2f\n", id_caixa, caixa_venda->valor_arrecadado);
    printf(" -> Produto: %s | Novo Estoque: %d\n", produto_vendido->nome, produto_vendido->estoque);

    free(produto_vendido);
    free(caixa_venda);
}

/* Não será mais necessário
void consultarProdutoPorId(FILE *arq_produtos, int id, FILE *log) {
    printf("\nConsultando produto com ID %d...\n", id);
    long pos = -1;
    fseek(arq_produtos, 0, SEEK_END);
    int total_produtos = ftell(arq_produtos) / sizeof(TProduto);
    TProduto *p = buscaBinariaProduto(id, arq_produtos, 1, total_produtos, &pos, log);

    if (p != NULL) {
        imprimeProd(p);
        free(p);
    } else {
        printf("Produto com ID %d nao encontrado.\n", id);
        fprintf(log, "\nConsulta: Produto com ID %d nao encontrado.\n", id);
    }
}
*/

void consultarFornecedorPorId(FILE *arq_fornecedores, int id, FILE *log) {
    printf("\nConsultando fornecedor com ID %d...\n", id);
    long pos = -1;
    fseek(arq_fornecedores, 0, SEEK_END);
    int total_fornecedores = ftell(arq_fornecedores) / sizeof(TFornecedor);
    TFornecedor *f = buscaBinariaFornecedor(id, arq_fornecedores, 1, total_fornecedores, &pos, log);

    if (f != NULL) {
        imprimeFornec(f);
        free(f);
    } else {
        printf("Fornecedor com ID %d nao encontrado.\n", id);
        fprintf(log, "\nConsulta: Fornecedor com ID %d nao encontrado.\n", id);
    }
}

void consultarCaixaPorId(FILE *arq_caixas, int id, FILE *log) {
    printf("\nConsultando caixa com ID %d...\n", id);
    long pos = -1;
    TCaixa *c = buscaSequencialCaixa(id, arq_caixas, &pos, log);

    if (c != NULL) {
        imprimeCaixa(c);
        free(c);
    } else {
        fprintf(log, "\nConsulta: Caixa com ID %d nao encontrado.\n", id);
        printf("\nErro durante a operaçao acesse o log para mais informaçoes\n");
    }
}

void atualizarProduto(FILE *arq_produtos, FILE *arq_fornecedores, FILE *arq_hash, FILE *log) {
    int id_produto;
    printf("\n### ATUALIZAÇÃO DE PRODUTO ###\n");
    printf("Digite o ID do produto que deseja atualizar: ");
    scanf("%d", &id_produto);
    while (getchar() != '\n'); // Limpa o buffer do teclado

    long pos_produto = -1;

    TProduto *p = buscaProdHash(id_produto, arq_produtos, arq_hash, &pos_produto);

    if (p == NULL) {
        printf("ERRO: Produto com ID %d não encontrado.\n", id_produto);
        return;
    }

    printf("\nProduto encontrado. Dados atuais:\n");
    imprimeProd(p);

    printf("\nDigite os novos dados do produto:\n");

    printf("Novo Nome: ");
    fgets(p->nome, sizeof(p->nome), stdin);
    p->nome[strcspn(p->nome, "\n")] = 0;

    printf("Nova Descrição: ");
    fgets(p->descricao, sizeof(p->descricao), stdin);
    p->descricao[strcspn(p->descricao, "\n")] = 0;

    printf("Novo Preço: ");
    scanf("%f", &p->preco);

    printf("Novo Estoque: ");
    scanf("%d", &p->estoque);
    while (getchar() != '\n');

    int fornecedor_valido = 0;
    while (!fornecedor_valido) {
        printf("Novo ID do Fornecedor: ");
        scanf("%d", &p->idFornecedor);
        while (getchar() != '\n');

        rewind(arq_fornecedores);
        TFornecedor *fornec_temp;
        while ((fornec_temp = leFornec(arq_fornecedores)) != NULL) {
            if (fornec_temp->id == p->idFornecedor) {
                fornecedor_valido = 1;
                free(fornec_temp);
                break;
            }
            free(fornec_temp);
        }
        if (!fornecedor_valido) {
            printf("ERRO: ID de fornecedor inválido. Tente novamente.\n");
        }
    }

    fseek(arq_produtos, pos_produto, SEEK_SET);
    salvaProd(p, arq_produtos);
    fflush(arq_produtos);

    printf("\nProduto com ID %d atualizado com sucesso!\n", id_produto);
    free(p);
}

void imprimirEstoqueBaixo(FILE *arquivo_produtos, int limite, FILE *log) {
    printf("\n--- RELATÓRIO DE PRODUTOS COM ESTOQUE BAIXO (Abaixo de %d) ---\n", limite);
    fprintf(log, "\n--- Relatorio de Estoque Baixo (Limite: %d) ---\n", limite);

    rewind(arquivo_produtos);
    TProduto *produto_atual;
    int produtos_encontrados = 0;

    while ((produto_atual = leProd(arquivo_produtos)) != NULL) {
        if (produto_atual->estoque < limite) {
            imprimeProd(produto_atual);
            fprintf(log, "ID: %d | Nome: %s | Estoque: %d\n", produto_atual->id, produto_atual->nome, produto_atual->estoque);
            produtos_encontrados++;
        }
        free(produto_atual);
    }

    if (produtos_encontrados == 0) {
        printf("\nNenhum produto com estoque baixo encontrado.\n");
        fprintf(log, "Nenhum produto com estoque baixo encontrado.\n");
    } else {
        printf("\nTotal de produtos com estoque baixo: %d\n", produtos_encontrados);
        fprintf(log, "Total de produtos com estoque baixo: %d\n", produtos_encontrados);
    }

    printf("--- FIM DO RELATÓRIO ---\n");
    fprintf(log, "--- Fim do Relatorio ---\n\n");
    fflush(log);
}
