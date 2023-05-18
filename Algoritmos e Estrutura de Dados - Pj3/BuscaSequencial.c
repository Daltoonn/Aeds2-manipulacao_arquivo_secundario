#include <stdio.h>
#include <stdlib.h>

typedef struct Funcionario{
    int codigo;
    char nome[50];
    float salario;
} TFuncionario;

TFuncionario buscaSequencial(int cod) {
    //fopen para abrir o arquivooooo aaaaaaaaaaaaaaaaaaaaaaaaa
    FILE *arquivo = fopen("funcionarios.dat", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo\n");
        exit(1);
    }

    TFuncionario *funcionario;
   //fread" é uma função da biblioteca da linguagem C  é usada para ler dados de um arquivo binário.
    while (fread(&funcionario, sizeof(TFuncionario), 1, arquivo) == 1) {
        if (funcionario->codigo == cod) {
            fclose(arquivo);
            return funcionario;
        }
    }

    fclose(arquivo);
    TFuncionario funcionario_vazio = {0, "", 0.0};
    return funcionario_vazio;
}
