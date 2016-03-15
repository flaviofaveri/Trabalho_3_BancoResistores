/* -------------------------------------------------------------------------

IFSC - Instituto Federal de Educa��o, Ci�ncia e Tecnologia de Santa Catarina
Curso Superior de Engenharia Eletr�nica
Programa��o de Computadores II
Prof.: Daniel Lohmann
Aluno: Flavio de Faveri
Data: 11/12/2014

Tarefa n�mero 2 de 2015

----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

// cria��o da estrutura n�
typedef struct node
{
    struct resistor* pResistor;  //resistor � a struct que descreve os itens que comp�em cada linha do arquivo
    struct node* pNodePrev;      //ponteiro aponta elemento anterior
    struct node* pNodeNext;      //ponteiro aponta proximo elemento
}Node;

// cria��o do cabe�alho
typedef struct header
{
    int sizeList;               //indica tamanho da lista
    Node* pNodeLast;            //aponta ultimo elemento da lista
    Node* pNodeFirst;           //aponta primeiro elemento da lista
}Header;

// cria��o da estrutura resistor
typedef struct resistor
{
    int serie;
    int potencia;
    int erro;
    int valor;
    int quantidade;
}Resistor;

Header* creatList();
void addLastList(Header* pHeader, Resistor* pResistorInserido);
int listLoad(Header* pHeader);
int listRecord(Header* pHeader);
void swapPointer(Node* pNode1, Node* pNode2);
void ordainList(Header* pHeader);
void printList(Header* pHeader);
Node* searchList(Node* pNode, int value);
void printWantedNode(Header* pHeader, int value);
void removeNodeList(Header* pHeader, int value);

int main()
{
    int opcao=0;
    int valorRes=0;
    Resistor* pResistorInserido = (Resistor*)malloc(sizeof(Resistor));
    Header* pHeader = creatList();
    listLoad(pHeader);
    while (1)
    {
        printf("\nBem vindo ao controle de estoque de Resistores!\n");
        printf("\nDigite <1> para listar o estoque completo");
        printf("\nDigite <2> para incluir um novo resistor");
        printf("\nDigite <3> para excluir um resistor");
        printf("\nDigite <4> para mostrar todos os resistores de um valor especifico");
        printf("\nDigite <5> para gravar alteracoes e sair\n");
        scanf("%d",&opcao);
        switch (opcao)
        {
            case 1:
                ordainList(pHeader);
                printList(pHeader);
                break;
            case 2:
                printf("\nDigite a serie do resistor a ser inserido: ");
                fflush(stdin);
                scanf("%d", &pResistorInserido->serie);
                printf("\nDigite a potencia do resistor a ser inserido: ");
                fflush(stdin);
                scanf("%d", &pResistorInserido->potencia);
                printf("\nDigite a tolerancia do resistor a ser inserido: ");
                fflush(stdin);
                scanf("%d", &pResistorInserido->erro);
                printf("\nDigite o valor do resistor a ser inserido: ");
                fflush(stdin);
                scanf("%d", &pResistorInserido->valor);
                printf("\nDigite a quantidade do resistor a ser inserido: ");
                fflush(stdin);
                scanf("%d", &pResistorInserido->quantidade);
                addLastList(pHeader, pResistorInserido);
                break;
            case 3:
                printf("\nDigite o valor de resistor que deseja excluir: ");
                fflush(stdin);
                scanf("%d", &valorRes);
                removeNodeList(pHeader, valorRes);
                break;
            case 4:
                printf("\nDigite o valor de resistor que deseja encontrar: ");
                fflush(stdin);
                scanf("%d", &valorRes);
                printWantedNode(pHeader, valorRes);
                break;
            case 5:
                ordainList(pHeader);
                listRecord(pHeader);
                exit(0);
                break;
        }
    }
    return 0;
}
/*Fun��o que cria a lista de resistores*/
Header *creatList()
{
    Header *pHeader = (Header*)malloc(sizeof(Header));
    pHeader->sizeList = 0;
    pHeader->pNodeLast = NULL;
    pHeader->pNodeFirst = NULL;
    return pHeader;
}

/*Fun��o que insere um novo n� no fim da lista*/
void addLastList(Header* pHeader, Resistor* pResistorInserido)
{
    Node *pNodeNew = (Node*)malloc(sizeof(Node));
    pNodeNew->pResistor = pResistorInserido;
    pNodeNew->pNodeNext = NULL;
    if (pHeader->pNodeFirst == NULL)
    {
        pHeader->pNodeFirst = pNodeNew;
    }
    if (pHeader->pNodeLast == NULL)
    {
        pHeader->pNodeLast = pNodeNew;
    }
    else
    {
        pHeader->pNodeLast->pNodeNext = pNodeNew;
        pNodeNew->pNodePrev = pHeader->pNodeLast;
        pHeader->pNodeLast = pNodeNew;
    }
    pHeader->sizeList++;
}

/*Fun��o que carrega o arquivo de resistores para a mem�ria em forma lista dupla encadeada*/
int listLoad(Header* pHeader)
{
    int n;
    FILE *fp;
    fp = fopen ("ListaResistores.txt", "r");
    if (fp == NULL)
    {
        printf ("Erro ao abrir o arquivo.\n");
        return 0;
    }
    do
    {
        Resistor* pResistor = (Resistor*)malloc(sizeof(Resistor));
        n=fscanf(fp, "%d %d %d %d %d\n", &pResistor->serie, &pResistor->potencia, &pResistor->erro, &pResistor->valor, &pResistor->quantidade);

        if (n == EOF)
            break;
        addLastList(pHeader, pResistor);
    }
    while(n != EOF);
    fclose (fp);
    return 1;
}

/*Fun��o que grava a lista de resistores que est� na mem�ria no arquivo*/
int listRecord(Header* pHeader)
{
    int n=0;
    Resistor* pResistorAtual;
    Node* pNodeAtual;
    FILE *fp;
    fp = fopen ("ListaResistores.txt", "w");
    if (fp == NULL)
    {
        printf ("Nao foi possivel gravar.\n");
        return 0;
    }
    for (n = 0; n < pHeader->sizeList; n++)
    {
        if (n == 0)
        {
            pNodeAtual = pHeader->pNodeFirst;
        }
        else
        {
            pNodeAtual = pNodeAtual->pNodeNext;
        }
        if (pNodeAtual == NULL)
        {
            break;
        }
        pResistorAtual = pNodeAtual->pResistor;
        fprintf(fp, "%d \t %d \t %d \t %d \t %d\n", pResistorAtual->serie, pResistorAtual->potencia, pResistorAtual->erro, pResistorAtual->valor, pResistorAtual->quantidade);
    }
    fclose(fp);
    return 1;
}

/*Fun��o que ordena a lista a partir da serie*/
void ordainList (Header* pHeader)
{
    int i=0, k=0;
    Node* pNode1;
    Node* pNode2;
    for (k=0; k<pHeader->sizeList; k++)
    {
        for (i=0; i<pHeader->sizeList-k-1; i++)
        {
            if (i == 0)
            {
                pNode1 = pHeader->pNodeFirst;
                pNode2 = pNode1->pNodeNext;
            }
            else
            {
                pNode1 = pNode2;
                pNode2 = pNode1->pNodeNext;
            }
            if (pNode1->pResistor->serie > pNode2->pResistor->serie)
                    swapPointer(pNode1, pNode2);
        }
    }
}

/*Fun��o que redireciona o ponteiro da estrutura resistor durante a ordena��o. � chamada dentro da fun��o ordena*/
void swapPointer (Node* pNode1, Node* pNode2)
{
    Resistor* pResAux;
    pResAux = pNode2->pResistor;
    pNode2->pResistor = pNode1->pResistor;
    pNode1->pResistor = pResAux;
}

/*Fun��o que imprime na tela toda a lista de resistores*/
void printList (Header* pHeader)
{
    Node* pNodeAux;
    for (pNodeAux = pHeader->pNodeFirst; pNodeAux != NULL; pNodeAux = pNodeAux->pNodeNext)
    {
        printf("E%d \t %d \t %d \t %d \t %d\n", pNodeAux->pResistor->serie, pNodeAux->pResistor->potencia, pNodeAux->pResistor->erro, pNodeAux->pResistor->valor, pNodeAux->pResistor->quantidade);
    }
}

/*Fun��o que busca o resistor de um valor especificado. Esta fun��o � chamada por outras para percorrer toda a lista em busca
do resistor e seus repetidos*/
Node* searchList (Node* pNode, int value)
{
    Node* pNodeAux;
    for (pNodeAux = pNode; pNodeAux != NULL; pNodeAux = pNodeAux->pNodeNext)
    {
        if (pNodeAux->pResistor->valor == value)
        {
            return pNodeAux;
        }
    }
    return NULL;
}

/*Fun��o que imprime na tela todos os resistores de um valor especificado independente dos outros par�metros*/
void printWantedNode (Header* pHeader, int value)
{
    Node* pNodePrint;
    pNodePrint = searchList(pHeader->pNodeFirst, value);
    if (pNodePrint != NULL)
    {
        printf("E%d \t %d \t %d \t %d \t %d\n", pNodePrint->pResistor->serie, pNodePrint->pResistor->potencia,
               pNodePrint->pResistor->erro, pNodePrint->pResistor->valor, pNodePrint->pResistor->quantidade);
    }
    else
    {
        return;
    }
    while (pNodePrint->pNodeNext != NULL)
    {
        pNodePrint = searchList(pNodePrint->pNodeNext, value);
        if (pNodePrint != NULL)
        {
            printf("E%d \t %d \t %d \t %d \t %d\n", pNodePrint->pResistor->serie, pNodePrint->pResistor->potencia,
                   pNodePrint->pResistor->erro, pNodePrint->pResistor->valor, pNodePrint->pResistor->quantidade);
        }
        else
        {
            return;
        }
    }
}

/*Fun��o que exclui todos os resistores de um valor expec�fico*/
void removeNodeList(Header* pHeader, int value)
{
    Node* pNodeRemoved;
    Node* pNodeAuxPrev;
    Node* pNodeAuxNext;
    for (pNodeRemoved = pHeader->pNodeFirst; pNodeRemoved->pNodeNext != NULL; pNodeRemoved = pNodeRemoved->pNodeNext)
    {
        pNodeRemoved = searchList(pHeader->pNodeFirst, value);
        if (pNodeRemoved != NULL)
        {
            if (pNodeRemoved == pHeader->pNodeFirst)
            {
                pNodeAuxNext = pNodeRemoved->pNodeNext;
                pHeader->pNodeFirst = pNodeAuxNext;
                pNodeAuxNext->pNodePrev = NULL;
            }
            else if (pNodeRemoved == pHeader->pNodeLast)
            {
                pNodeAuxPrev = pNodeRemoved->pNodePrev;
                pHeader->pNodeLast = pNodeAuxPrev;
                pNodeAuxPrev->pNodeNext = NULL;
            }
            else
            {
                pNodeAuxNext = pNodeRemoved->pNodeNext;
                pNodeAuxPrev = pNodeRemoved->pNodePrev;
                pNodeAuxPrev->pNodeNext = pNodeRemoved->pNodeNext;
                pNodeAuxNext->pNodePrev = pNodeRemoved->pNodePrev;
            }
        }
        else
            return;
        pHeader->sizeList--;
        free(pNodeRemoved);
    }
}
