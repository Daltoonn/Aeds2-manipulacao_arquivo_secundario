#include <stdio.h>
#include <stdlib.h>
#include "funcionario.c"
#include <time.h>

// UM DOS PROGRAMAS MAIS DIFICEIS QUE EU FIZ MEU DEUSSSSSSSSSSSSSSSSSSSSSSSSS kkkkkkkkkkkkkkkkkkkkkkkkkkk
void Inserir_Funcionarios(FILE *out){

    int cont = 0;
    int Funcionarios_Desordenados[5000];
    while(cont < 5000){
            //rand é uma função da biblioteca do C, que gera valores aleatorios
        int num = rand();
        int aux = 0;
        for(int i = 0; i < cont; i++){
            if(Funcionarios_Desordenados[i] == num){        // Compara todos os elementos do vetor para ver se será numero aleatorio
                aux = 1;  //Se for igual recebe 1
                break;
            }
        }
        if(!aux){                                           // Aux sendo igaul a 1, ele não entrará no while
            Funcionarios_Desordenados[cont] = num;
            cont++;
        }
    }

    //agora so atribuimos os valores do vet ao arquivo
    TFunc *f;
    for(int i = 0; i < 5000; i++){
        //o exercicio em questao exigiu apenas o CODIGO do funcionario e nao pediu nada em relação a outras especificações
        f = funcionario(Funcionarios_Desordenados[i],"X", "XXX.XXX.XXX-XX", "XX/XX/XXX", rand());
        salva(f, out);
    }
    free(f);
}

void le_funcionarios(FILE *in)
{
  printf("\n\nLendo funcionarios do arquivo...\n\n");
  rewind(in);
  TFunc *f;
  while ((f = le(in)) != NULL)
  {
    imprime(f);
    free(f);
  }
}


int main()
{
  // declara ponteiro para arquivo
  FILE *out;
  int chave;
  TFunc *f;

  // variaveis aux
  int codigof, comparacao, escolha;
  double tempoGasto = 0.0;

  //classificação interna particoes
  int nParticoes, nElementos;
  Lista *nomes = NULL;
  Lista *prox;
  //tem que colocar exatamente igual ao numero de funcionarios, se nao vai da ruim
  int nFunc=10;


  //intercalçao
  //usa as variaveis da intercalçao tambem
   Lista *nome_particao;

  // abre arquivo
  if ((out = fopen("funcionario.dat", "w+b")) == NULL)
  {
    printf("Erro ao abrir arquivo\n");
    exit(1);
  }
  else
  {

      // insere funcionários no programa
        Inserir_Funcionarios(out);


            int opcao;
            int aux=0;
            int menu;


        printf("\nBEM VINDO AO PROGRAMA!!");


        while(menu != 5){


        printf("\nDigite 1 - para mostrar a lista completa dos funcionarios");
        printf("\nDigite 2 - para buscar algum funcionario");
        printf("\nDigite 3 - para ordernar InserctionSort");
        printf("\nDigite 4 - para fazer particao dos arquivos");
        printf("\nDigite 5 - para sair do programa");
        printf("\n");
        scanf("%d",&opcao);

            switch (opcao)
                {
                case 1:
                        printf("\n\n===================LISTA DE TODOS OS FUNCIONARIOS!!!!===================\n");

                       // volta ao início do arquivo e lê os funcionários inseridos
                         le_funcionarios(out);
                         break;

                case 2:

                        printf("\nDigite o codigo do funcionario que deseja encontrar: ");
                        scanf("%i", &codigof);
                        clock_t begin = clock();
                        TFunc *func = buscaSequencial(codigof, out, &comparacao);
                        clock_t end = clock();
                        tempoGasto = (double)(end - begin) / CLOCKS_PER_SEC;

                        if (func == NULL)
                        {
                          printf("\nFuncionario nao encontrado.\n");
                          printf("\nTempo gasto na busca sequencial: %.5f segundos\n", tempoGasto);
                        }
                        else
                        {
                          printf("\nFuncionario encontrado: \n");
                          imprime(func);
                          printf("\nTempo gasto na busca sequencial: %.5f segundos\n", tempoGasto);
                        }
                        break;

                case 3:
                          //no metodo insertionSort usei a busca binaria para fazer a busca, pois os materiais disponibilizados me ajudaram demaissssss, dessa forma ja fiz a numero 2 e a 5
                          tempoGasto = 0.0;
                          printf("\nOrdenando com InsertionSort \n");
                          clock_t comeco = clock();
                          insertion_sort_disco(out, tamanho_arquivo(out));
                          clock_t fim = clock();
                          tempoGasto = (double)(fim - comeco) / CLOCKS_PER_SEC;
                          imprime_arquivo(out);
                          printf("\nTempo gasto no InsertionSort: %.5f segundos\n", tempoGasto);

                          //busca binaria
                          printf("\nUsando busca binaria!");
                          printf("\n\nDigite a chave a ser buscada: ");
                          fflush(stdin);
                          scanf("%d", &chave);
                          fflush(stdin);


                          clock_t comecoo = clock();
                          //usando busca binaria respondendo a questao 5
                          f = busca_binaria(chave, out, 0, tamanho_arquivo(out)-1);
                          imprime(f);
                          clock_t fimm = clock();
                          tempoGasto = (double)(fimm - comecoo) / CLOCKS_PER_SEC;
                          printf("\nTempo gasto no InsertionSort: %.5f segundos\n", tempoGasto);

                          break;




                 case 4:

                        printf("\nFAZENDO PARTICAO: ");


                        nParticoes = 10;
                        nElementos = 2;
                        printf("\n\nA estrutura de interpolacao tera 10 arquivos de no maximo 2 elementos\n");
                        nomes = cria("p1.dat", cria("p2.dat", cria("p3.dat", cria("p4.dat", cria("p5.dat", cria("p6.dat", cria("p7.dat", cria("p8.dat", cria("p9.dat", cria("p10.dat", NULL))))))))));
                        imprime(nomes);

                        //cria as particões que contém a base de dados de funcionários usando o método classificação interna
                        printf("\n\nGerando particoes, utilizando o metodo de classificacao interna.\n");
                        //salvando o ponteiro para o início da lista de nomes
                        prox = nomes;

                        clock_t beginn = clock();
                        classificacao_interna(out, nomes, nElementos, nFunc);
                        imprime(nomes);
                        clock_t endd = clock();
                        tempoGasto = (double)(endd - beginn) / CLOCKS_PER_SEC;

                        printf("\n==================================================================\n");
                        printf("\n\nFASE DA INTERCALACAO");


                        clock_t beginnn = clock();
                        intercalacao_basico("Arquivo intercalado.dat", nParticoes, nomes);

                        //fit so para intercalação
                        FILE *fit = fopen("arquivo_intercalado.dat", "rb+");

                        //que raivaaaaaaaaaaaaa tem que criar outraaa função para imprimir partiçoes aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
                        imprime_arquivo(out);
                        fclose(fit);

                        clock_t enddd = clock();
                        tempoGasto = (double)(enddd - beginnn) / CLOCKS_PER_SEC;
                        printf("\nTempo gasto na intercalacao: %.5f segundos\n", tempoGasto);



                        break;





                 case 5:
                         menu = 5;
                         break;

                default:
                          printf("Opcao invalida. Tente novamente.\n");
                          break;

                }


        }

    printf("\nVoce saiu do programa!");

    // fecha arquivo
    fclose(out);
    free(f);
    free(f);
  }

}
