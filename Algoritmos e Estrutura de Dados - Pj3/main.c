#include <stdio.h>
#include <stdlib.h>
#include "funcionario.c"
#include "Arvore_B.c"
#include "Hash.c"
#include <time.h>

int n = 1000;

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

void select_arvore(Node *no)
{
    int metodo_arvore = 0;
    printf("\nAgora Selecione uma opcao!!!");
    printf("\ndigite 1 Inserir na arvore b");
    printf("\ndigite 2 Remover na arvore b");
    printf("\ndigite 3 Procurar na arvore b");
    printf("\ndigite 4 Visualizar arvore b");
    printf("\ndigite 5 Voltar ao menu principal\n");
    scanf("%d", &metodo_arvore);

    //id
    int elemento = 0;

    //ponteiro para funcionarios arvore
    TFuncc *func = (TFuncc *)malloc(sizeof(TFunc));

    //ponteiros para codigo de funcionario
    Key_Sorting *key = (Key_Sorting *)malloc(sizeof(Key_Sorting));


//PROCURAR FUNCIONARIO SERVE PARA SALVAR E DEPOIS COM A FUNÇÃO ACHAR
    if (metodo_arvore != 4 && metodo_arvore != 1 && metodo_arvore != 5)
    {
        printf("Informe o code:\n");
        scanf("%d", &elemento);

        key->code = elemento;
    }

//SALVAR O CADASTRO
    if (metodo_arvore == 1)
    {
        fflush(stdin);
        printf("\nInforme o code: ");
        scanf("%d", &func->code);

        key->code = func->code;
        key->RRN = n;

        fflush(stdin);
        printf("\nInforme o nome: ");
        fgets(func->nome, 300, stdin);

        fflush(stdin);
        printf("\nInforme o nascimento: ");
        fgets(func->data_nascimento, 300, stdin);

        fflush(stdin);
        printf("\nInforme o salario: ");
        scanf("%d", &func->salario);
    }

    switch (metodo_arvore)
    {
    case 1:
        insert(&no, key, n, func);

        n = n + 1;

        system("cls");
        break;
    case 2:
        remove_tree(no, elemento);

        system("cls");
        break;
    case 3:
        search(no, elemento);
        break;
    case 4:
        toString(no);
        break;
    case 5:
        system("cls");

        return;
    default:
        printf("Valor invalido.");

        select_arvore(no);
        break;
    }

    free(key);
}


void Tabela_Hash()
{
                int numFunc = 1000;
                int numberOfPartition = 5;
                char partitionName[100];
                char str1[100];
                char str2[100] = ".dat";

                FILE *file = fopen("registro.dat", "wb+"); //criando arquivo para escrita
                int *vet;
                vet = (int *)malloc(numFunc * sizeof(int));
                embaralharVetor(vet, numFunc); //os identificadores serão embaralhados e distribuidos na quantidade de funcionários
                criarArquivo(file, numFunc, vet);


                particao_hash(numberOfPartition); //partição dos arquivos
                hashTable(file, numberOfPartition, numFunc); //criação da tabela hash

                printf("Tabela Hash criada com sucesso");

                int opcaoo = 0;

                while (opcaoo != 5) {

                    printf("\n");
                    printf("\ndigite 1 - Inserir funcionario");
                    printf("\ndigite 2 - Buscar Funcionario");
                    printf("\ndigite 3 - Deletar Funcionario");
                    printf("\ndigite 4 - Imprimir Tabela");
                    printf("\ndigite 5 - voltar ao menu");
                    printf("\n");
                    scanf("%i", &opcaoo);

                    if (opcaoo == 1) {
                        int employeeID;
                        int partitionID;

                        printf("\nDigite um identificador para o funcionario: ");
                        scanf("%i", &employeeID);

                        char auxName[15];
                        fflush(stdin);
                        printf("\nDigite um nome para o funcionario: ");
                        fgets(auxName,15,stdin);
                        fflush(stdin);


                        TFunccc funcHashInsert; //variável para novo registro de funcionário

                        funcHashInsert.id = employeeID; //apenas o identificador do novo registro

                        //criação do registro
                        sprintf(funcHashInsert.nome, "%s", auxName);
                        sprintf(funcHashInsert.cpf, "XXX.XXX.XXX-XX");
                        sprintf(funcHashInsert.dataNiver, "01/08/2021");
                        funcHashInsert.salario =rand();

                        partitionID = employeeID % numberOfPartition; //cálculo do resto de divisão

                        inserir_hash(&funcHashInsert, employeeID, partitionID); //inserindo funcionário na tabela hash


                    } else if (opcaoo == 2) {

                        int employeeID;
                        int partitionID;

                        printf("\nDigite o identificador para o funcionario: ");
                        scanf("%i", &employeeID);

                        partitionID = employeeID % numberOfPartition; //cálculo do resto da divisão

                        //chamando a função de busca
                        TFunccc *funcSearchHashTable = busca_hash(employeeID, partitionID); //realizando a busca na tabela hash

                        if (funcSearchHashTable != NULL) {
                            imprimirFuncionario(funcSearchHashTable);
                        } else {
                            printf("\nFuncionario nao encontrado");
                        }

                    } else if (opcaoo == 3) {

                        int employeeID;
                        int partitionID;

                        printf("\nDigite o identificador para o funcionario: ");
                        scanf("%i", &employeeID);

                        partitionID = employeeID % numberOfPartition; //cálculo de resto de divisão

                        deletar_hash(employeeID, partitionID); //excluindo funcionario da tabela hash

                    } else if (opcaoo == 4) {

                        for (int i = 0; i < numberOfPartition; ++i) {

                            itoa(i, str1, 10);
                            strcat(strcpy(partitionName, "hashTablePartition"), str1);
                            strcat(strcpy(partitionName, partitionName), str2);

                            FILE *filePartition = fopen(partitionName,"rb+"); //abertura do arquivo para leitura

                            imprimeParticao(filePartition, partitionName); //imprimindo todos os arquivos de partição da tabela hash

                            fclose(filePartition);
                        }

                    } else {
                        printf("\nvoltando ao menu\n");
                    }
                }

                fclose(file);
                return 0;
            }




int main()
{
   FILE *arq_file = fopen("binario_arvore_b.dat", "wb");

    TFunc *funcs = (TFunc *)malloc(sizeof(TFunc));

    funcs = (TFunc *)realloc(funcs, n * sizeof(TFunc));

//função é responsável por preencher a estrutura de dados
    createFunc(funcs, n);

//essa é a chave o programa root que nos guiar pela arvore fe
    Node *root = NULL;

//Essa função cria uma árvore a partir dos dados armazenados na estrutura TFunc e atualiza o ponteiro root para apontar para a raiz da árvore.
    createFuncWithTree(funcs, &root, n);

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


        while(menu != 7){


        printf("\nDigite 1 - para mostrar a lista completa dos funcionarios");
        printf("\nDigite 2 - para buscar algum funcionario");
        printf("\nDigite 3 - para ordernar InserctionSort");
        printf("\nDigite 4 - para fazer particao dos arquivos");
        printf("\nDigite 5 - para arvore B");
        printf("\nDigite 6 - para tabela Hash");
        printf("\nDigite 7 - para sair do programa");
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
                        select_arvore(root);

                        break;
                 case 6:
                        Tabela_Hash();

                        break;

                 case 7:
                         menu = 7;
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
