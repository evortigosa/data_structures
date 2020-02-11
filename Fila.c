/*      Algoritmos e Estruturas de Dados I - Filas
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

typedef struct NoFila {
    tipoItem item;
    struct NoFila *anterior;
    struct NoFila *proximo;
} TipoNoFila;

typedef TipoNoFila *Apontador;

typedef struct {
    Apontador primeiro;
    Apontador ultimo;
    int contador;
} TipoFila;

void inicializaFila(TipoFila *);
int filaVazia(TipoFila *);
int filaCheia(TipoFila *);
void enfileira(TipoFila *, tipoItem);
void desenfileira(TipoFila *, tipoItem *);
void frente(TipoFila *, tipoItem *);
void imprimeFila(TipoFila *);
void finalizaExecucao(TipoFila *);

int main(void) {
    int opcao= 1, aux= 0;
    TipoFila fila;
    tipoItem item;

    inicializaFila(&fila);

    while(opcao!= 0) {
        scanf("%d", &opcao);
        if (opcao== 1) scanf("%d", &aux);
        else aux= -1;
        item.chave= aux;

        if (opcao== 1) enfileira(&fila, item);
        else if (opcao== 2) desenfileira(&fila, &item);
        else if (opcao== 3) frente(&fila, &item);
        else if (opcao== 4) imprimeFila(&fila);

        if ((opcao> 1) && (opcao< 4)) {
            printf("\nChave: %d\n\n", item.chave);
            /* Demais campos do item... */
        }
    }
    finalizaExecucao(&fila);

return 0;

}

void inicializaFila(TipoFila *F) {
    F->primeiro= NULL;
    F->ultimo= NULL;
    F->contador= 0;
}

int filaVazia(TipoFila *F) {

    if (F->primeiro== NULL) return 1;
    return 0;
}

int filaCheia(TipoFila *F) {

    if (F->contador== MAX) return 1;
    return 0;
}

void enfileira(TipoFila *F, tipoItem I) {
    Apontador auxiliar;

    if (filaCheia(F)) printf("\nErro! Fila cheia!!!\n\n");
    else {
        auxiliar= (Apontador)malloc(sizeof(TipoNoFila));

        if (auxiliar== NULL) printf("\nErro! Memoria indisponivel!!!\n\n");
        else {
            if (F->ultimo== NULL) F->ultimo= auxiliar;          /* Inserção do primeiro elemento na fila */
            else F->primeiro->anterior= auxiliar;
            auxiliar->item= I;
            auxiliar->anterior= NULL;            /* Inserção do tipo primeiro elemento em lista ligada dupla */
            auxiliar->proximo= F->primeiro;
            F->primeiro= auxiliar;
            F->contador++;
        }
    }
}

void desenfileira(TipoFila *F, tipoItem *I) {
    Apontador auxiliar= F->ultimo;

    if (filaVazia(F)) printf("\nErro! Fila vazia!!!\n\n");
    else {
        (*I)= auxiliar->item;                    /* Remoção do tipo último elemento em lista ligada dupla */
        F->ultimo= auxiliar->anterior;
        if (auxiliar->anterior== NULL) F->primeiro= NULL;       /* Remoção do único elemento da fila */
        else auxiliar->anterior->proximo= auxiliar->proximo;    /* Remoção de elemento qualquer da fila */
        F->contador--;
        free(auxiliar);
    }
}

void frente(TipoFila *F, tipoItem *I) {

    if (filaVazia(F)) printf("\nErro! Fila vazia!!!\n\n");
    else (*I)= F->ultimo->item;                  /* Retorno do elemento na primeira posição da fila sem removê-lo */
}

void imprimeFila(TipoFila *F) {
    Apontador auxiliar= F->ultimo;

    if (filaVazia(F)) printf("\nFila vazia!!!\n\n");
    else {
        while(auxiliar!= NULL) {
            printf("\nChave: %d", auxiliar->item.chave);
            /* Demais campos do item... */
            auxiliar= auxiliar->anterior;        /* Impressão por ordem de desenfileiramento - FIFO */
        }
        printf("\n\n");
    }
}

void finalizaExecucao(TipoFila *F) {
    tipoItem item;

    while(!filaVazia(F)) desenfileira(F, &item);
}
