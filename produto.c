#include "produto.h"
#include <string.h>

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
    printf("R$%.2lf", produto->preco);
    printf("\nFornecedor de código: ");
    printf("%d", produto->idFornecedor);
    printf("\n**********************************************");
}

// Le um fornecedor do arquivo in na posicao atual do cursor
// Retorna um ponteiro para funcionario lido do arquivo
TProduto *leProd(FILE *in) {
    TProduto *prod = (TProduto *) malloc(sizeof(TProduto));
    if (0 >= fread(&prod->id, sizeof(int), 1, in)) {
        free(prod);
        return NULL;
    }
    fread(prod->nome, sizeof(char), sizeof(prod->nome), in);
    fread(prod->descricao, sizeof(char), sizeof(prod->descricao), in);
    fread(&prod->preco, sizeof(float), 1, in);
    fread(&prod->estoque, sizeof(int), 1, in);
    fread(&prod->idFornecedor, sizeof(int), 1, in);
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

    // Gerar o id com base na quantidade de produtos existente
    int novo_id = 1;
    TProduto *p_temp;
    rewind(out_produtos); // Garante que a leitura comece do início
    while ((p_temp = leProd(out_produtos)) != NULL) {
        if (p_temp->id >= novo_id) {
            novo_id = p_temp->id + 1;
        }
        free(p_temp);
    }

    printf("\n### CADASTRO DE NOVO PRODUTO ###\n");
    printf("O ID do novo produto será: %d\n", novo_id);

    // Solicita e armazena os dados do produto
    printf("Digite o nome do produto: ");
    fgets(nome, sizeof(nome), stdin);
    // Limpa o buffer pro proximo dado
    if(strchr(nome,'\n') == NULL){
        while ((temp_char = getchar()) != '\n' && temp_char != EOF){} //EOF é o fim do arquivo
    }
    nome[strcspn(nome, "\n")] = 0; //troca \n por \0



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

    // O fornecedor precisa ser verificado (p ver se existe)
    int fornecedor_valido = 0;
    //Loop p repetir caso invalido
    while (!fornecedor_valido) {
        //Pega o id do fornecedor
        printf("Digite o ID do fornecedor: ");
        scanf("%d", &idFornecedor);
        while ((temp_char = getchar()) != '\n' && temp_char != EOF){}



        TFornecedor *fornec_temp;
        rewind(in_fornecedores); // Volta ao início do arquivo de fornecedores para buscar
        //Faz a verificação
        while ((fornec_temp = leFornec(in_fornecedores)) != NULL) { //
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

    // salva no arquivo
    TProduto *p = produto(novo_id, nome, descricao, preco, estoque, idFornecedor); //

    fseek(out_produtos, 0, SEEK_END);
    salvaProd(p, out_produtos); //
    free(p);

    printf("\nProduto '%s' cadastrado com sucesso!\n", nome);
}
