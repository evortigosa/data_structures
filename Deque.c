/*      Algoritmos e Estruturas de Dados I - Deques
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

typedef struct NoDeque {
    tipoItem item;
    struct NoDeque *anterior;
    struct NoDeque *proximo;
} TipoNoDeque;

typedef TipoNoDeque *Apontador;

typedef struct {
    Apontador primeiro;
    Apontador ultimo;
    int contador;
} TipoDeque;

void inicializaDeque(TipoDeque *);
int dequeVazio(TipoDeque *);
int dequeCheio(TipoDeque *);
void insereInicio(TipoDeque *, tipoItem);
void insereFim(TipoDeque *, tipoItem);
void removeInicio(TipoDeque *, tipoItem *);
void removeFim(TipoDeque *, tipoItem *);
void frente(TipoDeque *, tipoItem *);
void back(TipoDeque *, tipoItem *);
void imprimeDeque(TipoDeque *);
void finalizaExecucao(TipoDeque *);

int main(void) {
    int opcao= 1, aux= 0;
    TipoDeque deque;
    tipoItem item;

    inicializaDeque(&deque);

    while(opcao!= 0) {
        scanf("%d", &opcao);
        if ((opcao== 1) || (opcao== 2)) scanf("%d", &aux);
        else aux= -1;
        item.chave= aux;

        if (opcao== 1) insereInicio(&deque, item);
        else if (opcao== 2) insereFim(&deque, item);
        else if (opcao== 3) removeInicio(&deque, &item);
        else if (opcao== 4) removeFim(&deque, &item);
        else if (opcao== 5) frente(&deque, &item);
        else if (opcao== 6) back(&deque, &item);
        else if (opcao== 7) imprimeDeque(&deque);

        if ((opcao> 2) && (opcao< 7)) {
            printf("\nChave: %d\n\n", item.chave);
            /* Demais campos do item... */
        }
    }
    finalizaExecucao(&deque);

return 0;

}

void inicializaDeque(TipoDeque *D) {
    D->primeiro= NULL;
    D->ultimo= NULL;
    D->contador= 0;
}

int dequeVazio(TipoDeque *D) {

    if ((D->primeiro== NULL) && (D->ultimo== NULL)) return 1;
    return 0;
}

int dequeCheio(TipoDeque *D) {

    if (D->contador== MAX) return 1;
    return 0;
}

void insereInicio(TipoDeque *D, tipoItem I) {
    Apontador auxiliar;

    if (dequeCheio(D)) printf("\nErro! Deque cheio!!!\n\n");
    else {
        auxiliar= (Apontador)malloc(sizeof(TipoNoDeque));

        if (auxiliar== NULL) printf("\nErro! Memoria indisponivel!!!\n\n");
        else {
            if (D->ultimo== NULL) D->ultimo= auxiliar;          /* Inserção do primeiro elemento no deque */
            else D->primeiro->anterior= auxiliar;
            auxiliar->item= I;
            auxiliar->anterior= NULL;            /* Inserção do tipo primeiro elemento em lista ligada dupla */
            auxiliar->proximo= D->primeiro;
            D->primeiro= auxiliar;
            D->contador++;
        }
    }
}

void insereFim(TipoDeque *D, tipoItem I) {
    Apontador auxiliar;

    if (dequeCheio(D)) printf("\nErro! Deque cheio!!!\n\n");
    else {
        auxiliar= (Apontador)malloc(sizeof(TipoNoDeque));

        if (auxiliar== NULL) printf("\nErro! Memoria indisponivel!!!\n\n");
        else {
            if (D->primeiro== NULL) D->primeiro= auxiliar;      /* Inserção do primeiro elemento no deque */
            else D->ultimo->proximo= auxiliar;
            auxiliar->item= I;
            auxiliar->anterior= D->ultimo;       /* Inserção do tipo último elemento em lista ligada dupla */
            auxiliar->proximo= NULL;
            D->ultimo= auxiliar;
            D->contador++;
        }
    }
}

void removeInicio(TipoDeque *D, tipoItem *I) {
    Apontador auxiliar= D->primeiro;

    if (dequeVazio(D)) printf("\nErro! Deque vazio!!!\n\n");
    else {
        (*I)= auxiliar->item;                    /* Remoção do tipo primeiro elemento em lista ligada dupla */
        D->primeiro= auxiliar->proximo;
        if (auxiliar->proximo== NULL) D->ultimo= NULL;          /* Remoção do único elemento do deque */
        else auxiliar->proximo->anterior= auxiliar->anterior;   /* Remoção de elemento qualquer no fim do deque */
        D->contador--;
        free(auxiliar);
    }
}

void removeFim(TipoDeque *D, tipoItem *I) {
    Apontador auxiliar= D->ultimo;

    if (dequeVazio(D)) printf("\nErro! Deque vazio!!!\n\n");
    else {
        (*I)= auxiliar->item;                    /* Remoção do tipo último elemento em lista ligada dupla */
        D->ultimo= auxiliar->anterior;
        if (auxiliar->anterior== NULL) D->primeiro= NULL;       /* Remoção do único elemento do deque */
        else auxiliar->anterior->proximo= auxiliar->proximo;    /* Remoção de elemento qualquer no fim do deque */
        D->contador--;
        free(auxiliar);
    }
}

void frente(TipoDeque *D, tipoItem *I) {

    if (dequeVazio(D)) printf("\nErro! Deque vazio!!!\n\n");
    else (*I)= D->primeiro->item;                /* Retorno do elemento na primeira posição do deque sem removê-lo */
}

void back(TipoDeque *D, tipoItem *I) {

    if (dequeVazio(D)) printf("\nErro! Deque vazio!!!\n\n");
    else (*I)= D->ultimo->item;                  /* Retorno do elemento na última posição do deque sem removê-lo */
}

void imprimeDeque(TipoDeque *D) {
    Apontador auxiliar= D->primeiro;

    if (dequeVazio(D)) printf("\nDeque vazio!!!\n\n");
    else {
        while(auxiliar!= NULL) {
            printf("\nChave: %d", auxiliar->item.chave);
            /* Demais campos do item... */
            auxiliar= auxiliar->proximo;         /* Impressão de elementos no sentido primeiro -> último */
        }
        printf("\n\n");
    }
}

void finalizaExecucao(TipoDeque *D) {
    tipoItem item;

    while(!dequeVazio(D)) removeInicio(D, &item);
}
