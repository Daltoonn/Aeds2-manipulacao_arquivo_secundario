#include <stdio.h>
#include <stdlib.h>

typedef struct Lista {
	char *nome;
	struct Lista *prox;
} Lista;


Lista *cria(char *nome, Lista *prox){
	Lista *nomes = (Lista *) malloc(sizeof(Lista));
	nomes->nome = nome;
	nomes->prox = prox;
	return nomes;
}
