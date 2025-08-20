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
        prod->prox = -1;
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

void imprimirBaseProd(FILE *arq_dados, FILE *arq_hash){
    printf("\nImprimindo a base de dados de produtos...\n");
    // 1. Percorre cada bucket da tabela hash
    for (int i = 0; i < TAM_HASH_PROD; i++) {
        THash bucket;
        fseek(arq_hash, i * sizeof(THash), SEEK_SET);
        fread(&bucket, sizeof(THash), 1, arq_hash);

        // 2. Se o bucket não estiver vazio, percorre a lista encadeada
        long pos_atual = bucket.pont;
        while (pos_atual != -1) {
            fseek(arq_dados, pos_atual, SEEK_SET);
            TProduto *p = leProd(arq_dados);
            if (p != NULL) {
                // 3. Imprime o produto encontrado na lista
                imprimeProd(p);

                // Avança para o próximo da lista
                long proximo_ponteiro = p->prox;
                free(p);
                pos_atual = proximo_ponteiro;
            } else {
                // Se der erro na leitura, para a lista atual
                break;
            }
        }
    }
    printf("\n--- Fim da lista de produtos ---\n");
}

void cadastrarProduto(FILE *out_produtos, FILE *out_hash, FILE *in_fornecedores) { //HASH
    char nome[100], descricao[256];
    float preco;
    int estoque, idFornecedor;
    int temp_char;
    int novo_id = 1;

    // Lógica para encontrar o próximo ID disponível
    fseek(out_produtos, 0, SEEK_END);
    if (ftell(out_produtos) > 0) {
        TProduto *p_temp;
        rewind(out_produtos);
        while ((p_temp = leProd(out_produtos)) != NULL) {
            if (p_temp->id >= novo_id) {
                novo_id = p_temp->id + 1;
            }
            free(p_temp);
        }
    }

    printf("\n### CADASTRO DE NOVO PRODUTO ###\n");
    printf("O ID do novo produto sera: %d\n", novo_id);
    while ((temp_char = getchar()) != '\n' && temp_char != EOF);

    printf("Digite o nome do produto: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = 0;

    printf("Digite a descricao do produto: ");
    fgets(descricao, sizeof(descricao), stdin);
    descricao[strcspn(descricao, "\n")] = 0;

    printf("Digite o preco do produto: ");
    scanf("%f", &preco);

    printf("Digite a quantidade em estoque: ");
    scanf("%d", &estoque);
    while ((temp_char = getchar()) != '\n' && temp_char != EOF);

    int fornecedor_valido = 0;
    while (!fornecedor_valido) {
        printf("Digite o ID do fornecedor: ");
        scanf("%d", &idFornecedor);
        while ((temp_char = getchar()) != '\n' && temp_char != EOF);

        // procura fornec p ver se é valido
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
            printf("\nERRO: Fornecedor com ID %d nao encontrado. Por favor, digite um ID valido.\n", idFornecedor);
        }
    }
    // =======================================================

    TProduto *p = produto(novo_id, nome, descricao, preco, estoque, idFornecedor);
    insereProdHash(p, out_produtos, out_hash);
    free(p);

    printf("\nProduto '%s' cadastrado com sucesso usando Tabela Hash!\n", nome);
}

//================================================================================
// INÍCIO TABELA HASH
//================================================================================

void criaHashProd(FILE *out) {
    printf("Criando arquivo de hash para produtos...\n");
    rewind(out);
    THash gaveta; //O THash é basicamente uma estrutura, com um int longo que vai armazenar o endereço em bytes do primeiro da lista, que vai ser a gaveta
    gaveta.pont = -1;
    for (int i = 0; i < TAM_HASH_PROD; i++) { //Vai criar várias gavetas vazias na tabela hash
        fwrite(&gaveta, sizeof(THash), 1, out);
    }
    printf("Arquivo de hash 'produtos.hash' criado com sucesso.\n");
}

int hashProd(int id) {
    return id % TAM_HASH_PROD; // basicamente a h(x) (x mod m)
}

long insereProdHash(TProduto *prod, FILE *arq_dados, FILE *arq_hash) {
    int pos_hash = hashProd(prod->id); //Pega a posição do produto na tabela
    THash gaveta;
    // Coloca o cursor na posição da tabela e lê para saber o ponteiro atual
    fseek(arq_hash, pos_hash * sizeof(THash), SEEK_SET);
    fread(&gaveta, sizeof(THash), 1, arq_hash);
    prod->prox = gaveta.pont; // Aponta para o antigo inicio da lista
    //Adiciona o produto na base
    fseek(arq_dados, 0, SEEK_END);
    long pos_novo_prod = ftell(arq_dados); //Pega o endereço do novo produto
    salvaProd(prod, arq_dados);
    //Define o novo inicio da lista/gaveta
    gaveta.pont = pos_novo_prod;
    fseek(arq_hash, pos_hash * sizeof(THash), SEEK_SET);
    fwrite(&gaveta, sizeof(THash), 1, arq_hash);
    return pos_novo_prod; // Retorna o endereço do novo produto
}

TProduto* buscaProdHash(int id, FILE *arq_dados, FILE *arq_hash, long *pos_encontrada) {
    int pos_hash = hashProd(id); //Pega qual "gaveta" agnt abre
    THash gaveta;
    fseek(arq_hash, pos_hash * sizeof(THash), SEEK_SET);
    fread(&gaveta, sizeof(THash), 1, arq_hash); //abre a gaveta

    if (gaveta.pont == -1) { //Se tiver vazia, retorna vazio
        *pos_encontrada = -1;
        return NULL;
    }

    long pos_atual = gaveta.pont; //pega o primeiro item da lista
    TProduto *prod;
    while (pos_atual != -1) {//Enquanto não for o ultimo produto...
        fseek(arq_dados, pos_atual, SEEK_SET);
        prod = leProd(arq_dados); //Lê o produto atual
        if (prod != NULL) {
            //Se achar...
            if (prod->id == id) {
                *pos_encontrada = pos_atual;
                return prod; //Retorna o que achou
            }
            //Se não achar vai caminhando pros próximos
            long proximo_ponteiro = prod->prox;
            free(prod);
            pos_atual = proximo_ponteiro;
        //Se não tiver arquivo ali
        } else {
            break;
        }
    }
    *pos_encontrada = -1;
    return NULL;
}

void removeProdHash(int id, FILE *arq_dados, FILE *arq_hash) {
    //Pega a gaveta...
    int pos_hash = hashProd(id);
    long pos_anterior = -1;
    THash gaveta;
    //Pega o primeiro da gaveta...
    fseek(arq_hash, pos_hash * sizeof(THash), SEEK_SET);
    fread(&gaveta, sizeof(THash), 1, arq_hash);
    long pos_atual = gaveta.pont;
    TProduto *prod_atual;

    while (pos_atual != -1) {
        //Coloca o cursor na posição atual
        fseek(arq_dados, pos_atual, SEEK_SET);
        prod_atual = leProd(arq_dados); //Produto atual recebe o produto da posição atual
        if (prod_atual && prod_atual->id == id) { //Se o id é igual ao procurado...
            if (pos_anterior == -1) { //Se o produto removido for o primeiro...
                //O primeiro vira o segundo
                gaveta.pont = prod_atual->prox;
                fseek(arq_hash, pos_hash * sizeof(THash), SEEK_SET);
                fwrite(&gaveta, sizeof(THash), 1, arq_hash); // salva na base de dados
            } else {
                //Se não for o primeiro, cria um produto anterior
                TProduto *prod_anterior = (TProduto*) malloc(sizeof(TProduto));
                fseek(arq_dados, pos_anterior, SEEK_SET);
                fread(prod_anterior, sizeof(TProduto), 1, arq_dados);
                //O produto sucessor vira o sucessor do antecessor e salva na base de dados
                prod_anterior->prox = prod_atual->prox;
                fseek(arq_dados, pos_anterior, SEEK_SET);
                salvaProd(prod_anterior, arq_dados);
                free(prod_anterior);
            }
            free(prod_atual);
            printf("Produto de ID %d removido com sucesso.\n", id);
            return;
        }
        pos_anterior = pos_atual; //Anterior vira atual
        if (prod_atual != NULL) {
            pos_atual = prod_atual->prox;
        } else {
            pos_atual = -1;
        }
        if(prod_atual != NULL){
            free(prod_atual);
        }
    }
    printf("Produto de ID %d nao encontrado para remocao.\n", id);
}

void consultarProdutoPorIdHash(FILE *arq_produtos, FILE* arq_hash, int id, FILE *log) {
    printf("\nConsultando produto com ID %d (usando Tabela Hash)...\n", id);
    long pos = -1;
    clock_t inicio = clock();
    TProduto *p = buscaProdHash(id, arq_produtos, arq_hash, &pos);
    clock_t fim = clock();
    double tempo_exec = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    if (p != NULL) {
        imprimeProd(p);
        free(p);
    } else {
        printf("Produto com ID %d nao encontrado.\n", id);
        fprintf(log, "\nConsulta (Hash): Produto com ID %d nao encontrado.\n", id);
    }
    fprintf(log, "Tempo de busca (Hash) para ID %d: %.6f segundos\n", id, tempo_exec);
    fflush(log);
}

/* Quicksort
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

*/

