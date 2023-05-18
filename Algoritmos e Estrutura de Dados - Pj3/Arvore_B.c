#include "arvore_b.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

//OPERAÇOESSSS COM A ARVOREEEEEEEEEEEEE B

//le funcionario
void leFuncc(TFuncc funcs)
{
    printf("Nome: %s\n", funcs.nome);
    printf("Nascimento: %s\n", funcs.data_nascimento);
    printf("Codigo: %d\n", funcs.code);
    printf("Salario:R$ %d\n", funcs.salario);
}

//cria funcionario
void createFunc(TFuncc *funcs, int n)
{
    int cont = 0;
    int Funcionarios_Desordenados[1000];
    while(cont < 1000){
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

    for (int j = 0; j < n; j++)
    {
        funcs[j].code = Funcionarios_Desordenados[j];
        sprintf(funcs[j].nome, "Funcionario N%d", Funcionarios_Desordenados[j]);
        funcs[j].salario = rand();
        strcpy(funcs[j].data_nascimento, "XX/XX/XXXX");
    }
}

//cria o arquivo
void createArq(TFuncc *funcs, FILE *arq, int n)
{
    if (arq != NULL)
    {
        for (int i = 0; i < n; i++)
        {
            fseek(arq, (i) * sizeof(TFuncc), SEEK_SET);
            fwrite(&funcs[i], sizeof(TFunc), 1, arq);
        }
    }

    fclose(arq);
}

//função da key cria arquivo binario, esta função grava um valor da estrutura TFuncc em um arquivo na posição especificada por i
void createArqByOne(TFuncc func, FILE *ordenado, int i)
{
    //verifica se ele não é nulo
    if (ordenado != NULL)
    {
        //vai para o inicio, e depois calcula a posicao correta
        fseek(ordenado, (i) * sizeof(TFuncc), SEEK_SET);
        //escreve no arquivo
        fwrite(&func, sizeof(TFuncc), 1, ordenado);
    }
}


//função é responsável por ler um registro do tipo TFuncc a partir de um arquivo binário, a partir de uma posição específica no arquivo
//Search CHAMA esta função e ela lê um valor da estrutura TFuncc a partir de um arquivo na posição especificada por i. que no caso é a função search de busca
TFuncc readFile(FILE *arq, int i)
{
    TFuncc func;

    //fseek posiciona
    fseek(arq, i * sizeof(TFuncc), SEEK_SET);
    fread(&func, sizeof(TFuncc), 1, arq);

    return func;
}

//le partir de um arquivo, lendo apenas o tamanho do valor
TFuncc readFileWithByte(FILE *arq)
{
    TFuncc func;
    //"fread" para ler o que está escrito naquele pedaço do arquivo
    fread(&func, sizeof(TFunc), 1, arq);

    return func;
}


//classificação chave
//é usado para organizar uma lista de coisas em ordem crescente com base no código
//fica comparando uma com outra para ORDENARRRR
void keySorting(FILE *arq, FILE *ordenado, int tamanho)
{
    double time_spent = 0.0;

    clock_t begin = clock();

    Key_Sorting array[tamanho];

//posiciona para o fim
    rewind(arq);

//percorre o arquivo binário e preenche o array com os códigos e a posição de cada registro no arquivo
    for (int i = 0; i < tamanho; i++)
    {
        fseek(arq, i * sizeof(TFuncc), SEEK_SET);
        array[i].RRN = ftell(arq);
        TFuncc func = readFile(arq, i);
        array[i].code = func.code;
    }

//código ordena o array de acordo com os códigos das funções , compara qual é maior que qual
    for (int i = 0; i < tamanho; i++)
    {
        for (int j = i + 1; j < tamanho; j++)
        {
            if (array[i].code > array[j].code)
            {
                Key_Sorting aux = array[i];
                array[i] = array[j];
                array[j] = aux;
            }
        }
    }

    rewind(arq);
//o, usando a posição de cada registro no array ordenado para ler os registros do arquivo em ordem e escrevê-los em um novo arquivo ordenado usando a função "createArqByOne()".
    for (int i = 0; i < tamanho; i++)
    {
        fseek(arq, array[i].RRN, SEEK_SET);
        TFuncc func = readFileWithByte(arq);

        createArqByOne(func, ordenado, i);
    }

    clock_t end = clock();

    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;

    printf("\nTempo de processamento do key sorting: %.2f\n\n", time_spent);

    fclose(arq);
}



////////////////
///////////////////////
//FUNCOESSSSSSSSSSSSSSS DA ARVOREEEEEEE


//cria um novo nó para uma árvore binária de busca. A função recebe como parâmetro um ponteiro para um objeto Key_Sorting
//preenchendo-o com uma cópia da chave passada como parâmetro e alocando a memória necessária para o novo nó e sua chave.
//peguei do codigo e do slide da tatiana
Node *criaNo(Key_Sorting *x)
{
    // função aloca memória para um novo nó e para uma nova chave dentro do nó
    Node *novo_no = (Node *)malloc(sizeof(Node));
    novo_no->key = (Key_Sorting *)malloc(sizeof(Key_Sorting));
    novo_no->key->code = x->code;
    novo_no->key->RRN = x->RRN;
    novo_no->left = NULL;
    novo_no->right = NULL;
    return (novo_no);
};

//particiona se nao de certo
//insere um nó em uma árvore binária de busca, representada pelo ponteiro ppRaiz
int insert(Node **ppRaiz, Key_Sorting *x, int n, TFuncc *func)
{

    //a função cria um arquivo binário com as informações contidas em func
    if (*ppRaiz == NULL)
    {
        FILE *arq_file = fopen("binario_arvore_b.dat", "rb+");

        createArqByOne(*func, arq_file, n);

        fclose(arq_file);

        //cria um novo nó usando a função criaNo() e atribui o ponteiro desse nó para ppRaiz.
        *ppRaiz = criaNo(x);

        return 1;
    }

   //particiona
   //Se a árvore não estiver vazia, função verifica se o código contido no nó a ser inserido é menor que o código contido no nó atual
   //Se for maior vai para esquerda Left , se for maior Direita
    if (x->code < (*ppRaiz)->key->code)
        return insert(&((*ppRaiz)->left), x, n, func);
    if (x->code > (*ppRaiz)->key->code)
        return insert(&((*ppRaiz)->right), x, n, func);
    return 0;
};


// é uma função que cria uma árvore binária de busca a partir de um conjunto de dados contido em um vetor de estruturas do tipo TFuncc.
// o segundo parametro que esqueso que fez eu ter raiva demaisss significa um ponteiro para um ponteiro para um nó da árvore o segundo
void createFuncWithTree(TFuncc *funcs, Node **no, int n)
{

    //ciar um ponteiro para uma estrutura
    Key_Sorting *key = (Key_Sorting *)malloc(sizeof(Key_Sorting));

    //função percorre todo o vetor de
    for (int j = 0; j < n; j++)
    {
        key->code = funcs[j].code;
        key->RRN = j;
        //verificar se o nó raiz é nulo, caso seja, ela cria um novo nó com a chave e o RRN passados como parâmetros e insere os dados em um arquivo binário utilizando a função createArqByOne
        insert(no, key, j, &funcs[j]);
    }
}

//busca mais facil
//recursividadeeee
int search(Node *no, int code)
{
    if (no == NULL)
    {
        printf("Arvore nula!");

        return 0;
    }
    //se for menor vai para esquerda
    else if (code < no->key->code)
    {
        return search(no->left, code);
    }
    //se for maior vai para a direita
    else if (code > no->key->code)
    {
        return search(no->right, code);
    }
    else
    {
        //se estiver no atual deu bommmmm
        printf("\nCodigo %d encontrado no indice %d!\n\n", no->key->code, no->key->RRN);

        printf("\n********* INFORMACOES DO FUNCIONARIO %d ENCONTRADO ********* !\n\n", no->key->code);

        FILE *arq_file = fopen("binario_arvore_b.dat", "rb+");

        //ativa funcao de ler
        TFuncc func = readFile(arq_file, no->key->RRN);

        leFuncc(func);

        fclose(arq_file);

        return 1;
    }
}


//imprime na tela as informações armazenadas nos nós de uma árvore binária de busca ordenadas em ordem crescente de code.
//recebe um ponteiro para a raiz da árvore como entrada
void toString(Node *root)
{
    if (root == NULL)
    {
        return;
    }
    else
    {
        //imprime da esquerda para a direita ordem crescenteee
        toString(root->left);
        printf("\nCode: %d\n", root->key->code
               );
        printf("Indice: %d\n", root->key->RRN);
        toString(root->right);
    }
}


//removeerr, mais facil do que parece kkkkkkk
//O parâmetro root é a raiz da árvore, e id é o código do nó a ser removido vamos tirar um no
//quando chega no null tira e deixa desconexo
void remove_tree(Node *root, int id)
{
    //prox = pai
    Node *prox = NULL;
    Node *atual = root;

    //usa o while para percorrer ate achar ou nao kkkkkk
    while (atual != NULL)
    {
        // verifica se o nó atual é o nó a ser removido
        if (atual->key->code == id)
        {
            // verifica se o nó atual é o nó a ser removido no caso ele é uma folha
            if (atual->left == NULL && atual->right == NULL)
            {
                // verifica se o nó atual é o nó a ser removido
                if (prox == NULL)
                {
                    //retira o no
                    root = NULL;
                }
                else
                {
                    // se nao vai para esquerda ou direita verifica se o nó atual é o nó a ser removido
                    if (prox->left == atual)
                    {
                        prox->left = NULL;
                    }
                    else
                    {
                        prox->right = NULL;
                    }
                }

            }
             // se o nó a ser removido tem APENAS um filho,
            else if (atual->left == NULL)
            {
                // se o nó a ser removido é a raiz da árvore
                if (prox == NULL)
                {
                    //filho substitui o nó a ser removido.
                    root = atual->right;
                }
                else
                {
                   // atualiza o pai do nó a ser removido para apontar para o seu filho à direita ou esquerda
                    if (prox->left == atual)
                    {
                        prox->left = atual->right;
                    }
                    else
                    {
                        prox->right = atual->right;
                    }
                }
            }
            // se o nó a ser removido tem DOISSSSS filhoss, primeiramente copia e cola o de cima
            else if (atual->right == NULL)
            {
                // se o nó a ser removido é a raiz da árvor
                if (prox == NULL)
                {
                    root = atual->left;
                }
                else
                {
                   // atualiza o pai do nó a ser removido para apontar para o seu filho à direita ou esquerda
                    if (prox->left == atual)
                    {
                        prox->left = atual->left;
                    }
                    else
                    {
                        prox->right = atual->left;
                    }
                }
            }
            // se o nó a ser removido é a raiz da árvor
            else
            {
                // encontra o sucessor do nó a ser removido (menor nó da subárvore à direita)
                Node *successor = atual->right;

                // para o próprio nó a ser removido
                Node *successor_parent = atual;



                // código atualiza o ponteiro "successor_parent" para apontar para o ponteiro "successor" e atualiza o ponteiro "successor"
                //para apontar para o filho esquerdo do nó sucessor atual.
                while (successor->left != NULL)
                {
                    successor_parent = successor;
                    successor = successor->left;
                }

                // encontra o sucessor do nó a ser removido (menor nó da subárvore à direita)
                if (successor_parent->left == successor)
                {
                    successor_parent->left = successor->right;
                }
                else
                {
                    successor_parent->right = successor->right;
                }

                // substitui a chave do nó a ser removido pela chave do sucessor
                atual->key = successor->key;
            }

            // substitui a chave do nó a ser removido pela chave do sucessor
            free(atual);

            return;
        }

        //quando o nó atual não contém a chave de busca

       // Se o código da chave do nó atual for maior do que o código da chave de busca
       //significa que a chave de busca deve estar no subárvore esquerdo ou direito do nó atual
        else if (atual->key->code > id)
        {
            prox = atual;
            atual = atual->left;
        }
        else
        {
            prox = atual;
            atual = atual->right;
        }

    }//FIM DO WHILE

}


