/*      Algoritmos e Estruturas de Dados I - Pilhas
        Evandro Scudeleti Ortigossa, n° USP 6793135
        Bacharelado em Ciências de Computação 2009        */

#include <stdio.h>
#include <stdlib.h>

#define MAX 1000

typedef int tipoChave;

typedef struct {
    tipoChave chave;
    /* Demais campos do item... */
} tipoItem;

typedef struct NoPilha {
    tipoItem item;
    struct NoPilha *proximo;
} TipoNoPilha;

typedef TipoNoPilha *Apontador;

typedef struct {
    Apontador primeiro;
    int contador;
} TipoPilha;

void inicializaPilha(TipoPilha *);
int pilhaVazia(TipoPilha *);
int pilhaCheia(TipoPilha *);
void push(TipoPilha *, tipoItem);
void pop(TipoPilha *, tipoItem *);
void topo(TipoPilha *, tipoItem *);
void imprimePilha(TipoPilha *);
void finalizaExecucao(TipoPilha *);

int main(void) {
    int opcao= 1, aux= 0;
    TipoPilha pilha;
    tipoItem item;

    inicializaPilha(&pilha);

    while(opcao!= 0) {
        scanf("%d", &opcao);
        if (opcao== 1) scanf("%d", &aux);
        else aux= -1;
        item.chave= aux;

        if (opcao== 1) push(&pilha, item);
        else if (opcao== 2) pop(&pilha, &item);
        else if (opcao== 3) topo(&pilha, &item);
        else if (opcao== 4) imprimePilha(&pilha);

        if ((opcao> 1) && (opcao< 4)) {
            printf("\nChave: %d\n\n", item.chave);
            /* Demais campos do item... */
        }
    }
    finalizaExecucao(&pilha);

return 0;

}

void inicializaPilha(TipoPilha *P) {
    P->primeiro= NULL;
    P->contador= 0;
}

int pilhaVazia(TipoPilha *P) {

    if (P->primeiro== NULL) return 1;
    return 0;
}

int pilhaCheia(TipoPilha *P) {

    if (P->contador== MAX) return 1;
    return 0;
}

void push(TipoPilha *P, tipoItem I) {
    Apontador auxiliar;

    if (pilhaCheia(P)) printf("\nErro! Pilha cheia!!!\n\n");
    else {
        auxiliar= (Apontador)malloc(sizeof(TipoNoPilha));

        if (auxiliar== NULL) printf("\nErro! Memoria indisponivel!!!\n\n");
        else {
            auxiliar->item= I;
            auxiliar->proximo= P->primeiro;      /* Inserção do tipo primeiro elemento em lista ligada simples */
            P->primeiro= auxiliar;
            P->contador++;
        }
    }
}

void pop(TipoPilha *P, tipoItem *I) {
    Apontador auxiliar;

    if (pilhaVazia(P)) printf("\nErro! Pilha vazia!!!\n\n");
    else {
        auxiliar= P->primeiro;
        (*I)= auxiliar->item;
        P->primeiro= auxiliar->proximo;          /* Remoção do tipo primeiro elemento em lista ligada simples */
        P->contador--;
        free(auxiliar);
    }
}

void topo(TipoPilha *P, tipoItem *I) {

    if (pilhaVazia(P)) printf("\nErro! Pilha vazia!!!\n\n");
    else (*I)= P->primeiro->item;                /* Retorno do elemento no topo da pilha sem removê-lo */
}

void imprimePilha(TipoPilha *P) {
    Apontador auxiliar= P->primeiro;

    if (pilhaVazia(P)) printf("\nPilha vazia!!!\n\n");
    else {
        while(auxiliar!= NULL) {
            printf("\nChave: %d", auxiliar->item.chave);
            /* Demais campos do item... */
            auxiliar= auxiliar->proximo;         /* Impressão por ordem de desempilhamento - LIFO */
        }
        printf("\n\n");
    }
}

void finalizaExecucao(TipoPilha *P) {
    tipoItem item;

    while(!pilhaVazia(P)) pop(P, &item);
}
