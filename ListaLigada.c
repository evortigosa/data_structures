/*    Algoritmos e Estruturas de Dados I - Listas Encadeadas
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
    struct NoLista *proximo;
} TipoNoLista;

typedef TipoNoLista *Apontador;

typedef struct {
    Apontador primeiro;
    Apontador ultimo;
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
    L->ultimo= NULL;
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
            auxiliar->item= I;
            auxiliar->proximo= L->primeiro;
            L->primeiro= auxiliar;
            if (L->ultimo== NULL) L->ultimo= auxiliar;
            L->contador++;
        }
    }
}

void insereUltimoLista(TipoLista *L, tipoItem I) {
    Apontador auxiliar;

    if (listaCheia(L)) printf("\nErro! Lista cheia!!!\n\n");
    else {
        auxiliar= (Apontador)malloc(sizeof(TipoNoLista));

        if (auxiliar== NULL) printf("\nErro! Memoria indisponivel!!!\n\n");
        else {
            auxiliar->item= I;
            auxiliar->proximo= NULL;
            if (L->primeiro== NULL) L->primeiro= auxiliar;
            else L->ultimo->proximo= auxiliar;
            L->ultimo= auxiliar;
            L->contador++;
        }
    }
}

void removeLista(TipoLista *L, tipoChave chave) {
    Apontador auxiliar, auxExclusao;

    if (listaVazia(L)) printf("\nErro! Lista vazia!!!\n\n");
    else {
        auxiliar= buscaElemento(L, chave);

        if (auxiliar== NULL) printf("\nErro! Elemento nao encontrado!!!\n\n");
        else {
            if (auxiliar->item.chave== chave) {                 /* Remoção do primeiro ou único elemento */
                if (auxiliar== L->ultimo) L->ultimo= NULL;      /* Caso de elemento único na lista */
                L->primeiro= auxiliar->proximo;
                auxExclusao= auxiliar;
            }
            else {                          /* Caso geral */
                if (auxiliar->proximo== L->ultimo) L->ultimo= auxiliar;   /* Remoção do último elemento da lista */
                auxExclusao= auxiliar->proximo;
                auxiliar->proximo= auxiliar->proximo->proximo;
            }
            L->contador--;
            free(auxExclusao);
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
            if (auxiliar->item.chave!= chave)    /* O elemento retornado pode ser o primeiro, mas pesquisado o segundo. Portanto, */
                auxiliar= auxiliar->proximo;     /* não posso usar (auxiliar== L->primeiro) para separar estes casos */

            printf("\nChave: %d", auxiliar->item.chave);
            /* Demais campos do item... */
            printf("\n\n");
        }
    }
}

Apontador buscaElemento(TipoLista *L, tipoChave chave) {
    Apontador auxiliar= L->primeiro;

    if (auxiliar->item.chave== chave) return auxiliar;
    while(auxiliar->proximo!= NULL) {
        if (auxiliar->proximo->item.chave== chave) return auxiliar;

        auxiliar= auxiliar->proximo;
    }
    return NULL;
}

void imprimeLista(TipoLista *L) {
    Apontador auxiliar= L->primeiro;

    if (listaVazia(L)) printf("\nLista vazia!!!\n\n");
    else {
        while(auxiliar!= NULL) {
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
