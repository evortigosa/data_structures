/*  Algoritmos e Estruturas de Dados I - Listas Encadeadas Circulares
              Evandro Scudeleti Ortigossa, n° USP 6793135
              Bacharelado em Ciências de Computação 2009            */

#include <stdio.h>
#include <stdlib.h>

#define MAX 1000

typedef int tipoChave;

typedef struct {
    tipoChave chave;
    /* Demais campos do item... */
} tipoItem;

typedef struct NoLista {
    tipoItem item;
    struct NoLista *anterior;
    struct NoLista *proximo;
} TipoNoLista;

typedef TipoNoLista *Apontador;

typedef struct {
    Apontador primeiro;
    int contador;
} TipoLista;

void inicializaLista(TipoLista *);
int listaVazia(TipoLista *);
int listaCheia(TipoLista *);
void inserePrimeiroLista(TipoLista *, tipoItem);
void insereUltimoLista(TipoLista *, tipoItem);
void removeLista(TipoLista *, tipoChave);
void pesquisa(TipoLista *, tipoChave);
Apontador buscaElemento(TipoLista *, tipoChave);
void imprimeLista(TipoLista *);
void finalizaExecucao(TipoLista *);

int main(void) {
    int opcao= 1, aux= 0;
    TipoLista lista;
    tipoItem item;

    inicializaLista(&lista);

    while(opcao!= 0) {
        scanf("%d", &opcao);
        if (opcao!= 0 && opcao< 5) scanf("%d", &aux);
        item.chave= aux;

        if (opcao== 1) inserePrimeiroLista(&lista, item);
        else if (opcao== 2) insereUltimoLista(&lista, item);
        else if (opcao== 3) removeLista(&lista, aux);
        else if (opcao== 4) pesquisa(&lista, aux);
        else if (opcao== 5) imprimeLista(&lista);
    }
    finalizaExecucao(&lista);

return 0;

}

void inicializaLista(TipoLista *L) {
    L->primeiro= NULL;
    L->contador= 0;
}

int listaVazia(TipoLista *L) {

    if (L->primeiro== NULL) return 1;
    return 0;
}

int listaCheia(TipoLista *L) {

    if (L->contador== MAX) return 1;
    return 0;
}

void inserePrimeiroLista(TipoLista *L, tipoItem I) {
    Apontador auxiliar;

    if (listaCheia(L)) printf("\nErro! Lista cheia!!!\n\n");
    else {
        auxiliar= (Apontador)malloc(sizeof(TipoNoLista));

        if (auxiliar== NULL) printf("\nErro! Memoria indisponivel!!!\n\n");
        else {
            if (listaVazia(L)) {
                auxiliar->anterior= auxiliar;    /* Inserção do primeiro elemento na lista */
                auxiliar->proximo= auxiliar;
            }
            else {
                L->primeiro->anterior->proximo= auxiliar;
                auxiliar->anterior= L->primeiro->anterior;      /* Ligação do novo elemento com o primeiro da lista e do último */
                auxiliar->proximo= L->primeiro;                 /* da lista com o novo elemento - forma circular */
                L->primeiro->anterior= auxiliar;
            }
            auxiliar->item= I;
            L->primeiro= auxiliar;
            L->contador++;
        }
    }
}

void insereUltimoLista(TipoLista *L, tipoItem I) {

    if (listaCheia(L)) printf("\nErro! Lista cheia!!!\n\n");
    else {
        inserePrimeiroLista(L, I);          /* Último elemento de uma lista circular é o elemento imediatamente anterior ao */
        L->primeiro= L->primeiro->proximo;  /* primeiro elemento da lista. Portanto basta inserir na primeira posição e mover */
    }                                       /* o ponteiro para a próxima posição, que era o primeiro antes da inserção */
}

void removeLista(TipoLista *L, tipoChave chave) {
    Apontador auxiliar;

    if (listaVazia(L)) printf("\nErro! Lista vazia!!!\n\n");
    else {
        auxiliar= buscaElemento(L, chave);

        if (auxiliar== NULL) printf("\nErro! Elemento nao encontrado!!!\n\n");
        else {
            if (L->primeiro->proximo== auxiliar) inicializaLista(L);           /* Caso em que o elemento é único na lista */
            else {
                if (L->primeiro== auxiliar) L->primeiro= auxiliar->proximo;    /* Remoção do primeiro elemento da lista */
                auxiliar->proximo->anterior= auxiliar->anterior;
                auxiliar->anterior->proximo= auxiliar->proximo;                /* Não há necessidade de tratar caso de remoção */
                L->contador--;                                                 /* do último elemento em uma lista circular */
            }
            free(auxiliar);
        }
    }
}

void pesquisa(TipoLista *L, tipoChave chave) {
    Apontador auxiliar;

    if (listaVazia(L)) printf("\nErro! Lista vazia!!!\n\n");
    else {
        auxiliar= buscaElemento(L, chave);

        if (auxiliar== NULL) printf("\nElemento nao encontrado!!!\n\n");
        else {
            printf("\nChave: %d", auxiliar->item.chave);
            /* Demais campos do item... */
            printf("\n\n");
        }
    }
}

Apontador buscaElemento(TipoLista *L, tipoChave chave) {
    Apontador auxiliar= L->primeiro;

    if (auxiliar->item.chave== chave) return auxiliar;          /* Assume-se aqui que a verificação de lista não vazia já tenha */
    auxiliar= auxiliar->proximo;                                /* sido feita antes da chamada da função busca */

    while(auxiliar!= L->primeiro) {                             /* Em uma lista circular não há NULL indicando seu final */
        if (auxiliar->item.chave== chave) return auxiliar;      /* Necessidade de elemento fixo como referência de ciclo */

        auxiliar= auxiliar->proximo;
    }
    return NULL;
}

void imprimeLista(TipoLista *L) {
    Apontador auxiliar= L->primeiro;

    if (listaVazia(L)) printf("\nLista vazia!!!\n\n");
    else {
        printf("\nChave: %d", auxiliar->item.chave);
        /* Demais campos do item... */
        auxiliar= auxiliar->proximo;

        while(auxiliar!= L->primeiro) {
            printf("\nChave: %d", auxiliar->item.chave);
            /* Demais campos do item... */
            auxiliar= auxiliar->proximo;
        }
        printf("\n\n");
    }
}

void finalizaExecucao(TipoLista *L) {

    while(!listaVazia(L)) removeLista(L, (L->primeiro->item.chave));
}
