/*  Algoritmos e Estruturas de Dados I - Listas Encadeadas Duplas
           Evandro Scudeleti Ortigossa, n° USP 6793135
           Bacharelado em Ciências de Computação 2009            */

#include <stdio.h>
#include <stdlib.h>

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
typedef Apontador TipoLista;

void inicializaLista(TipoLista *);
int listaVazia(TipoLista *);
void insereLista(TipoLista *, tipoItem);
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
        if (opcao!= 0 && opcao< 4) scanf("%d", &aux);
        item.chave= aux;

        if (opcao== 1) insereLista(&lista, item);
        else if (opcao== 2) removeLista(&lista, aux);
        else if (opcao== 3) pesquisa(&lista, aux);
        else if (opcao== 4) imprimeLista(&lista);
    }
    finalizaExecucao(&lista);

return 0;

}

void inicializaLista(TipoLista *L) {
    (*L)= NULL;
}

int listaVazia(TipoLista *L) {

    if ((*L)== NULL) return 1;
    return 0;
}

void insereLista(TipoLista *L, tipoItem I) {
    Apontador auxiliar;

    auxiliar= (Apontador)malloc(sizeof(TipoNoLista));

    if (auxiliar== NULL) printf("\nErro! Memoria indisponivel!!!\n\n");
    else {
        if ((*L)!= NULL) (*L)->anterior= auxiliar;         /* Inserção do primeiro elemento na lista */
        auxiliar->item= I;
        auxiliar->anterior= NULL;
        auxiliar->proximo= *L;
        (*L)= auxiliar;
    }
}

void removeLista(TipoLista *L, tipoChave chave) {
    Apontador auxiliar;

    if (listaVazia(L)) printf("\nErro! Lista vazia!!!\n\n");
    else {
        auxiliar= buscaElemento(L, chave);

        if (auxiliar== NULL) printf("\nErro! Elemento nao encontrado!!!\n\n");
        else {
            if (auxiliar->anterior== NULL) {               /* Remoção do primeiro elemento da lista */
                (*L)= auxiliar->proximo;
                if ((*L)!= NULL) (*L)->anterior= NULL;     /* Caso em que o elemento é único na lista */
            }
            else {           /* Remoção de elemento qualquer da lista */
                if (auxiliar->proximo!= NULL) auxiliar->proximo->anterior= auxiliar->anterior;     /* Caso o elemento não for o último */
                auxiliar->anterior->proximo= auxiliar->proximo;
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
    Apontador auxiliar= *L;

    while(auxiliar!= NULL) {
        if (auxiliar->item.chave== chave) return auxiliar;

        auxiliar= auxiliar->proximo;
    }
    return NULL;
}

void imprimeLista(TipoLista *L) {
    Apontador auxiliar= *L;

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

    while(!listaVazia(L)) removeLista(L, ((*L)->item.chave));
}
