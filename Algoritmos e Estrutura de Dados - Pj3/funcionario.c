#include "funcionario.h"
#include "arvore_b.h"
#include "particoes.c"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

// Imprime funcionario
void imprime(TFunc *func)
{
  printf("\n");
  printf("\nFuncionario de codigo ");
  printf("%d", func->cod);
  printf("\nNome: ");
  printf("%s", func->nome);
  printf("\nCPF: ");
  printf("%s", func->cpf);
  printf("\nData de Nascimento: ");
  printf("%s", func->data_nascimento);
  printf("\nSalario:R$ ");
  printf("%4.2f", func->salario);
  printf("\n");
}

// Cria funcionario. Lembrar de usar free(func)
TFunc *funcionario(int cod, char *nome, char *cpf, char *data_nascimento, double salario)
{
  TFunc *func = (TFunc *)malloc(sizeof(TFunc));
  // inicializa espaço de memória com ZEROS
  if (func)
    memset(func, 0, sizeof(TFunc));
  // copia valores para os campos de func
  func->cod = cod;
  strcpy(func->nome, nome);
  strcpy(func->cpf, cpf);
  strcpy(func->data_nascimento, data_nascimento);
  func->salario = salario;
  return func;
}

// Salva funcionario no arquivo out, na posicao atual do cursor
void salva(TFunc *func, FILE *out)
{
  fwrite(&func->cod, sizeof(int), 1, out);
  // func->nome ao invés de &func->nome, pois string já é ponteiro
  fwrite(func->nome, sizeof(char), sizeof(func->nome), out);
  fwrite(func->cpf, sizeof(char), sizeof(func->cpf), out);
  fwrite(func->data_nascimento, sizeof(char), sizeof(func->data_nascimento), out);
  fwrite(&func->salario, sizeof(double), 1, out);
}

TFunc *le(FILE *in)
{
  TFunc *func = (TFunc *)malloc(sizeof(TFunc));

  //"fread" é uma função da  linguagem C que é usada para ler dados de um arquivo binário
  if (0 >= fread(&func->cod, sizeof(int), 1, in))
  {
    free(func);
    return NULL;
  }
  fread(func->nome, sizeof(char), sizeof(func->nome), in);
  fread(func->cpf, sizeof(char), sizeof(func->cpf), in);
  fread(func->data_nascimento, sizeof(char), sizeof(func->data_nascimento), in);
  fread(&func->salario, sizeof(double), 1, in);
  return func;
}

 // Imprime arquivo
void imprime_arquivo(FILE *arq)
{
  // le o arquivo e coloca no vetor
  rewind(arq); // posiciona cursor no inicio do arquivo
  TFunc *f = le(arq);



  // feof retorna um valor diferente de zero se uma operação de leitura tentou ler após o final do arquivo
  while (!feof(arq))
  {
    imprime(f);
    f = le(arq);
  }
}

// Retorna tamanho do funcionario em bytes
int tamanho()
{
  return sizeof(int)         // cod
         + sizeof(char) * 50 // nome
         + sizeof(char) * 15 // cpf
         + sizeof(char) * 11 // data_nascimento
         + sizeof(double);   // salario
}

int tamanho_arquivo(FILE *arq)
{
  fseek(arq, 0, SEEK_END);
  int tam = round((double)ftell(arq) / tamanho());
  return tam;
}

TFunc *buscaSequencial(int id, FILE *out, int *comparacao)
{
  int position = 0;

  rewind(out);

  while (!feof(out))
  {

    fseek(out, position * sizeof(TFunc), SEEK_SET);
    TFunc *func = le(out);

    position++;

    if (func == NULL)
    {
      *comparacao += 1;
      return NULL;
    }
    else if (id == func->cod)
    {
      *comparacao += 1;
      return func;
    }
    else
    {
      *comparacao += 1;
    }
  }

  return NULL;
}

void insertion_sort_disco(FILE *out, int tam)
{
  int i;
  // faz o insertion sort
  for (int j = 2; j <= tam; j++)
  {
    // posiciona o arquivo no registro j
    fseek(out, (j - 1) * tamanho(), SEEK_SET);
    TFunc *fj = le(out);
//    printf("\n********* Funcionario atual: %d\n", fj->cod);
    i = j - 1;
    // posiciona o cursor no registro i
    fseek(out, (i - 1) * tamanho(), SEEK_SET);
    TFunc *fi = le(out);
//    printf("fi = %d\n", fi->cod);
    while ((i > 0) && (fi->cod > fj->cod))
    {
      // posiciona o cursor no registro i+1
      fseek(out, i * tamanho(), SEEK_SET);
//      printf("Salvando funcionario %d na posicao %d\n", fi->cod, i + 1);
      salva(fi, out);
      i = i - 1;
      // lÃª registro i
      fseek(out, (i - 1) * tamanho(), SEEK_SET);
      fi = le(out);
//      printf("fi = %d; i = %d\n", fi->cod, i);
    }
    // posiciona cursor no registro i + 1
    fseek(out, (i)*tamanho(), SEEK_SET);
//    printf("*** Salvando funcionario %d na posicao %d\n", fj->cod, i + 1);
    // salva registro j na posiÃ§Ã£o i
    salva(fj, out);
  }
  // descarrega o buffer para ter certeza que dados foram gravados
  fflush(out);
}


//busca binaria usar depois do insertion sort
TFunc *busca_binaria(int chave, FILE *in, int inicio, int fim) {
    TFunc *f = NULL;
    int cod = -1;
    int contador=0;


    while (inicio <= fim && cod != chave) {
        int meio = trunc((inicio + fim) / 2);


        contador++;
        printf("Inicio: %d; Fim: %d; Meio: %d; comparacao numero: %d\n", inicio, fim, meio,contador);

  //"fread" é uma função da  linguagem C que é usada para ler dados de um arquivo binário
        fseek(in, (meio -1) * tamanho(), SEEK_SET);
        f = le(in);
        cod = f->cod;
        if (f) {
            if (cod > chave) {
                fim = meio - 1;
            } else {
                inicio = meio + 1;
            }
        }
    }
    if (cod == chave) {
        return f;
    }
    else return NULL;
}


//PARTIÇÕESSSSSSSSSSSS
void classificacao_interna(FILE *in, Lista *nome_arquivos_saida, int M, int nFunc) {
    rewind(in); //posiciona cursor no inicio do arquivo


    int reg = 0;

    while (reg != nFunc) {
        //le o arquivo e coloca no vetor
        TFunc *v[M];
        int i = 0;
        while (!feof(in)) {
            fseek(in, (reg) * tamanho(), SEEK_SET);
            v[i] = le(in);
       //     imprime_funcionario(v[i]);
            i++;
            reg++;
            if(i>=M) break;
        }

        //ajusta tamanho M caso arquivo de entrada tenha terminado antes do vetor
        if (i != M) {
            M = i;
        }

        //faz ordenacao
        for (int j = 1; j < M; j++) {
            TFunc *f = v[j];
            i = j - 1;
            while ((i >= 0) && (v[i]->cod > f->cod)) {
                v[i + 1] = v[i];
                i = i - 1;
            }
            v[i + 1] = f;
        }

        //cria arquivo de particao e faz gravacao
        char *nome_particao = nome_arquivos_saida->nome;
        nome_arquivos_saida = nome_arquivos_saida->prox;
        printf("\n%s\n", nome_particao);
        FILE *p;
        if ((p = fopen(nome_particao, "wb+")) == NULL) {
            printf("Erro criar arquivo de saida\n");
        } else {
            for (int i = 0; i < M; i++) {
                fseek(p, (i) * tamanho(), SEEK_SET);
                salva(v[i], p);
                imprime(v[i]);
            }
            fclose(p);
        }
        for(int jj = 0; jj<M; jj++)
            free(v[jj]);
    }
}


//INTERCACAOO
//estrutura para armazenar o vetor de partições
typedef struct vetor{
    TFunc *func;
    int aux_p;
    FILE *f;
}TVet;

void intercalacao_basico(char *nome_arquivo_saida, int num_p, Lista *nome_particoes) {

    int fim = 0; //variavel que controla fim do procedimento
    FILE *out; //declara ponteiro para arquivo

    //abre arquivo de saida para escrita
    if ((out = fopen(nome_arquivo_saida, "wb")) == NULL) {
        printf("Erro ao abrir arquivo de sa?da\n");
    } else {
        //cria vetor de particoes
        TVet v[num_p];

        //abre arquivos das particoes, colocando variavel de arquivo no campo f do vetor
        //e primeiro funcionario do arquivo no campo func do vetor
        for (int i=0; i < num_p; i++) {
            v[i].f = fopen(nome_particoes->nome, "rb");
            v[i].aux_p = 0;
            if (v[i].f != NULL) {
                fseek(v[i].f, v[i].aux_p * tamanho(), SEEK_SET);
                TFunc *f = le(v[i].f);
                if (f == NULL) {
                    //arquivo estava vazio
                    //coloca HIGH VALUE nessa posi??o do vetor
                    v[i].func = funcionario(INT_MAX, "","","",0);
                }
                else {
                    //conseguiu ler funcionario, coloca na posi??o atual do vetor
                    v[i].func = f;
                }
            }
            else {
                fim = 1;
            }
            nome_particoes = nome_particoes->prox;
        }

        int aux = 0;
        while (!(fim)) { //conseguiu abrir todos os arquivos
            int menor = INT_MAX;
            int pos_menor;
            //encontra o funcionario com menor chave no vetor
            for(int i = 0; i < num_p; i++){
                if(v[i].func->cod < menor){
                    menor = v[i].func->cod;
                    pos_menor = i;
                }
            }
            if (menor == INT_MAX) {
                fim = 1; //terminou processamento
            }
            else {
                //salva funcionario no arquivo de saÃ¬da
                fseek(out, aux * tamanho(), SEEK_SET);
                salva(v[pos_menor].func, out);
                //atualiza posiÃ§Ã£o pos_menor do vetor com pr?ximo funcionario do arquivo
                v[pos_menor].aux_p++;
                fseek(v[pos_menor].f, v[pos_menor].aux_p * tamanho(), SEEK_SET);
                TFunc *f = le(v[pos_menor].f);
                aux++;
                if (f == NULL) {
                    //arquivo estava vazio
                    //coloca HIGH VALUE nessa posiÃ§ao do vetor
                    v[pos_menor].func = funcionario(INT_MAX, "", "", "",0.0);
                }
                else {
                    v[pos_menor].func = f;
                }

            }
        }

        //fecha arquivos das partiÃ‡Ãµes de entrada
        for(int i = 0; i < num_p; i++){
            fclose(v[i].f);
        //    free(v[i].func);
        }
        //fecha arquivo de saÃ¬da
        fclose(out);
    }

//so para imprimirrr a intercalação aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa


}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//arvoreeeeeeeeee

//OPERAÇOESSSS COM A ARVOREEEEEEEEEEEEE B
//OPERAÇOESSSS COM A ARVOREEEEEEEEEEEEE B
