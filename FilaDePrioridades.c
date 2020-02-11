/*      Algoritmos e Estruturas de Dados I - Filas de Prioridades
              Evandro Scudeleti Ortigossa, n° USP 6793135
              Bacharelado em Ciências de Computação 2009         */

#include <stdio.h>
#include <stdlib.h>

#define MAX 1000

typedef int tipoPrioridade;

typedef struct {
    tipoPrioridade prioridade;
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
void reordenaFila(TipoFila *, Apontador);
void redefinePrioridade(TipoFila *, tipoPrioridade, tipoPrioridade);
Apontador buscaElemento(TipoFila *, tipoPrioridade);
void imprimeFila(TipoFila *);
void finalizaExecucao(TipoFila *);

int main(void) {
    int opcao= 1, aux= 0, nova= 0;
    TipoFila fila;
    tipoItem item;

    inicializaFila(&fila);

    while(opcao!= 0) {
        scanf("%d", &opcao);
        if ((opcao== 1) || (opcao== 4)) {
            scanf("%d", &aux);
            if (opcao== 4) scanf("%d", &nova);
        }
        else aux= -1;
        item.prioridade= aux;

        if (opcao== 1) enfileira(&fila, item);
        else if (opcao== 2) desenfileira(&fila, &item);
        else if (opcao== 3) frente(&fila, &item);
        else if (opcao== 4) redefinePrioridade(&fila, aux, nova);
        else if (opcao== 5) imprimeFila(&fila);

        if ((opcao> 1) && (opcao< 4)) {
            printf("\nPrioridade: %d\n\n", item.prioridade);
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

    if ((F->primeiro== NULL) && (F->ultimo== NULL)) return 1;
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

            reordenaFila(F, (F->primeiro));      /* Fila é reordenada por critérios de prioridade */
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

void reordenaFila(TipoFila *F, Apontador posicao) {
    tipoItem auxItem;

    if (posicao->anterior!= NULL) {
        while((posicao->anterior!= NULL) && ((posicao->item.prioridade)< (posicao->anterior->item.prioridade))) {
            auxItem= posicao->item;
            posicao->item= posicao->anterior->item;   /* Ordenação da fila segundo prioridades, elemento */
            posicao->anterior->item= auxItem;         /* com maior prioridade vai pra frente da fila */

            posicao= posicao->anterior;
        }
    }                                  /* Após redefinição da prioridade, elemento pode ser movido para frente ou para trás */
    if (posicao->proximo!= NULL) {
        while((posicao->proximo!= NULL) && ((posicao->item.prioridade)> (posicao->proximo->item.prioridade))) {
            auxItem= posicao->item;
            posicao->item= posicao->proximo->item;
            posicao->proximo->item= auxItem;

            posicao= posicao->proximo;
        }
    }
}

void redefinePrioridade(TipoFila *F, tipoPrioridade prioridade, tipoPrioridade novaPrioridade) {
    Apontador auxiliar;

    if (filaVazia(F)) printf("\nErro! Fila vazia!!!\n\n");
    else {
        auxiliar= buscaElemento(F, prioridade);

        if (auxiliar== NULL) printf("\nErro! Elemento nao encontrado!!!\n\n");
        else {
            auxiliar->item.prioridade= novaPrioridade;     /* Elemento alterado e fila é reordenada por critérios de prioridade */
            reordenaFila(F, auxiliar);
        }
    }
}

Apontador buscaElemento(TipoFila *F, tipoPrioridade prioridade) {
    Apontador auxiliar= F->primeiro;

    while(auxiliar!= NULL) {
        if (auxiliar->item.prioridade== prioridade) return auxiliar;

        auxiliar= auxiliar->proximo;
    }
    return NULL;
}

void imprimeFila(TipoFila *F) {
    Apontador auxiliar= F->ultimo;

    if (filaVazia(F)) printf("\nFila vazia!!!\n\n");
    else {
        while(auxiliar!= NULL) {
            printf("\nPrioridade: %d", auxiliar->item.prioridade);
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
