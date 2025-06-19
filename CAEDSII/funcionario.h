#ifndef FUNCIONARIO_H_INCLUDED
#define FUNCIONARIO_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>

typedef struct Funcionario {
    int cod;
    char nome[50];
    char cpf[15];
    char data_nascimento[11];
    double salario;
} TFunc;

typedef struct {
    double texec;
    int comparacoes, trocas;
} Tmetrica;

// Retorna tamanho do funcionario em bytes
int tamanho_registro();

// Cria funcionario.
TFunc *funcionario(int cod, char *nome, char *cpf, char *data_nascimento, double salario);


// Salva funcionario no arquivo out, na posicao atual do cursor
void salva(TFunc *func, FILE *out);


// retorna a quantidade de registros no arquivo
int tamanho_arquivo(FILE *arq);


// Le um funcionario do arquivo in na posicao atual do cursor
// Retorna um ponteiro para funcionario lido do arquivo
TFunc *le(FILE *in);


// Imprime funcionario
void imprime(TFunc *func);


// Cria a base de dados
void criarBase(FILE *out, int tam);

//embaralha base de dados
void shuffle(int *vet,int MAX,int MIN);

void embaralha(int *vet, int tam);

// Imprime a base de dados
void imprimirBase(FILE *out);


// Compara dois funcionarios
// Retorna 1 se os valores dos atributos de ambos forem iguais
// e 0 caso contrario
int compara(TFunc *c1, TFunc *c2);




#endif // FUNCIONARIO_H_INCLUDED
