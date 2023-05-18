#ifndef FUNCIONARIO_H_INCLUDED
#define FUNCIONARIO_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

typedef struct Funcionario
{
  int cod;
  char nome[50];
  char cpf[15];
  char data_nascimento[11];
  double salario;
} TFunc;

typedef struct File{
    TFunc *func;
    long init_p;
    long end_p;
    FILE *filePartition;
}TVetFile;


// Imprime funcionario
void imprime(TFunc *func);

// Cria funcionario. Lembrar de usar free(funcionario)
TFunc *funcionario(int cod, char *nome, char *cpf, char *data_nascimento, double salario);

// Salva funcionario no arquivo out, na posicao atual do cursor
void salva(TFunc *func, FILE *out);

// Le um funcionario do arquivo in na posicao atual do cursor
TFunc *le(FILE *in);

// Retorna tamanho do funcionario em bytes
int tamanho();

int tamanho_arquivo(FILE *arq);

// Realiza a busca sequencial
TFunc *buscaSequencial(int id, FILE *out, int *comparacao);

// Realiza a ordenação insertionSort
void insertion_sort_disco(FILE *out, int tam);

void imprime_arquivo(FILE *out);

// Realiza a bisca binária
TFunc *busca_binaria(int chave, FILE *in, int inicio, int fim);

#endif // FUNCIONARIO_H_INCLUDED
