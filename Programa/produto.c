#include "produto.h"
#include <string.h>
#include <time.h>

TProduto *produto(int id, char *nome, char *descricao, float preco, int estoque, int idFornecedor){
    TProduto *prod = (TProduto *) malloc(sizeof(TProduto));
    if (prod) {
        memset(prod, 0, sizeof(TProduto));
        prod->id = id;
        strncpy(prod->nome, nome, 100);
        strncpy(prod->descricao, descricao, 256);
        prod->preco = preco;
        prod->estoque = estoque;
        prod->idFornecedor = idFornecedor;
    }
    return prod;
}

void salvaProd(TProduto *prod, FILE *out) {
    fwrite(prod, sizeof(TProduto), 1, out);
}

void imprimeProd(TProduto *produto) {
    printf("\n**********************************************");
    printf("\nProduto de código: ");
    printf("%d", produto->id);
    printf("\nNome: ");
    printf("%s", produto->nome);
    printf("\nDescrição: ");
    printf("%s", produto->descricao);
    printf("\nPreço unitário: ");
    printf("R$%.2f", produto->preco);
    printf("\nEstoque: ");
    printf("%d", produto->estoque);
    printf("\nFornecedor de código: ");
    printf("%d", produto->idFornecedor);
    printf("\n**********************************************");
}

TProduto *leProd(FILE *in) {
    TProduto *prod = (TProduto *) malloc(sizeof(TProduto));
    if (prod == NULL) {
        perror("Erro de alocacao de memoria para produto");
        return NULL;
    }
    if (fread(prod, sizeof(TProduto), 1, in) != 1) {
        free(prod);
        return NULL;
    }
    return prod;
}

void imprimirBaseProd(FILE *out){
    printf("\nImprimindo a base de dados de produtos...\n");
    rewind(out);
    TProduto *p;
    while ((p = leProd(out)) != NULL){
        imprimeProd(p);
        free(p);
    }
}

void cadastrarProduto(FILE *out_produtos, FILE *in_fornecedores) {
    char nome[100], descricao[256];
    float preco;
    int estoque, idFornecedor;
    int temp_char;
    int novo_id = 1;
    TProduto *p_temp;

    rewind(out_produtos);
    while ((p_temp = leProd(out_produtos)) != NULL) {
        if (p_temp->id >= novo_id) {
            novo_id = p_temp->id + 1;
        }
        free(p_temp);
    }

    printf("\n### CADASTRO DE NOVO PRODUTO ###\n");
    printf("O ID do novo produto será: %d\n", novo_id);

    printf("Digite o nome do produto: ");
    fgets(nome, sizeof(nome), stdin);
    if(strchr(nome,'\n') == NULL){
        while ((temp_char = getchar()) != '\n' && temp_char != EOF){}
    }
    nome[strcspn(nome, "\n")] = 0;

    printf("Digite a descrição do produto: ");
    fgets(descricao, sizeof(descricao), stdin);
    if(strchr(descricao,'\n') == NULL)
        while ((temp_char = getchar()) != '\n' && temp_char != EOF){}
    descricao[strcspn(descricao, "\n")] = 0;

    printf("Digite o preço do produto: ");
    scanf("%f", &preco);

    printf("Digite a quantidade em estoque: ");
    scanf("%d", &estoque);
    while ((temp_char = getchar()) != '\n' && temp_char != EOF){}

    int fornecedor_valido = 0;
    while (!fornecedor_valido) {
        printf("Digite o ID do fornecedor: ");
        scanf("%d", &idFornecedor);
        while ((temp_char = getchar()) != '\n' && temp_char != EOF){}

        TFornecedor *fornec_temp;
        rewind(in_fornecedores);
        while ((fornec_temp = leFornec(in_fornecedores)) != NULL) {
            if (fornec_temp->id == idFornecedor) {
                fornecedor_valido = 1;
                free(fornec_temp);
                break;
            }
            free(fornec_temp);
        }

        if (!fornecedor_valido) {
            printf("\nERRO: Fornecedor com ID %d não encontrado. Por favor, digite um ID válido.\n", idFornecedor);
        }
    }

    TProduto *p = produto(novo_id, nome, descricao, preco, estoque, idFornecedor);
    fseek(out_produtos, 0, SEEK_END);
    salvaProd(p, out_produtos);
    free(p);

    printf("\nProduto '%s' cadastrado com sucesso!\n", nome);
}

static void trocarProd(FILE *arq, int i, int j, TMetrica *metricas) {
    if (i == j) return;
    TProduto *p_i = (TProduto*) malloc(sizeof(TProduto));
    TProduto *p_j = (TProduto*) malloc(sizeof(TProduto));

    fseek(arq, i * sizeof(TProduto), SEEK_SET);
    fread(p_i, sizeof(TProduto), 1, arq);
    fseek(arq, j * sizeof(TProduto), SEEK_SET);
    fread(p_j, sizeof(TProduto), 1, arq);

    fseek(arq, i * sizeof(TProduto), SEEK_SET);
    fwrite(p_j, sizeof(TProduto), 1, arq);
    fseek(arq, j * sizeof(TProduto), SEEK_SET);
    fwrite(p_i, sizeof(TProduto), 1, arq);

    metricas->trocas++;
    free(p_i);
    free(p_j);
}

static int particionaProd(FILE *arq, int baixo, int alto, TMetrica *metricas) {
    TProduto *pivo = (TProduto*) malloc(sizeof(TProduto));
    fseek(arq, alto * sizeof(TProduto), SEEK_SET);
    fread(pivo, sizeof(TProduto), 1, arq);

    int i = (baixo - 1);
    TProduto *p_j = (TProduto*) malloc(sizeof(TProduto));

    for (int j = baixo; j <= alto - 1; j++) {
        fseek(arq, j * sizeof(TProduto), SEEK_SET);
        fread(p_j, sizeof(TProduto), 1, arq);
        metricas->comparacoes++;
        if (p_j->id < pivo->id) {
            i++;
            trocarProd(arq, i, j, metricas);
        }
    }
    trocarProd(arq, i + 1, alto, metricas);
    free(pivo);
    free(p_j);
    return (i + 1);
}

static void quicksortRecursivoProd(FILE *arq, int baixo, int alto, TMetrica *metricas) {
    if (baixo < alto) {
        int pi = particionaProd(arq, baixo, alto, metricas);
        quicksortRecursivoProd(arq, baixo, pi - 1, metricas);
        quicksortRecursivoProd(arq, pi + 1, alto, metricas);
    }
}

void quicksortProd(FILE *arq, int tam, FILE *log) {
    if (tam <= 1) return;
    TMetrica metricas = {0.0, 0, 0};
    clock_t inicio = clock();

    quicksortRecursivoProd(arq, 0, tam - 1, &metricas);

    clock_t fim = clock();
    metricas.texec = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    fflush(arq);

    fprintf(log, "\n--- Metricas Quicksort (Produtos) ---\n");
    fprintf(log, "Comparacoes: %d\n", metricas.comparacoes);
    fprintf(log, "Trocas: %d\n", metricas.trocas);
    fprintf(log, "Tempo de Execucao: %.4f segundos\n", metricas.texec);
    fflush(log);
}
