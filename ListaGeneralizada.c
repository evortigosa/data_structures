/*      Algoritmos e Estruturas de Dados I - Listas Generalizadas
              Evandro Scudeleti Ortigossa, n° USP 6793135
              Bacharelado em Ciências de Computação 2009         */

#include <stdio.h>
#include <stdlib.h>

typedef int tipoChave;

typedef struct {
    tipoChave chave;
    /* Demais campos do item... */
} tipoItem;

typedef struct NoLista {
    int tag;                           /* TAG: 0 (CABEÇA é átomo) ou 1 (CABEÇA é ponteiro) */
    union {
        struct NoLista *carLista;      /* CAR (CABEÇA): um átomo ou um ponteiro para uma outra lista */
        tipoItem carAtomo;
    } item;
    struct NoLista *cdr;               /* CDR (CAUDA): ligação para a cauda da lista (próximo elemento) */
} TipoNoLista;

typedef TipoNoLista *Apontador;
typedef Apontador TipoLista;

void inicializaLista(TipoLista *);
int listaVazia(TipoLista *);
void pesquisa(TipoLista *, tipoChave);
Apontador buscaAtomo(TipoLista *, tipoChave);
Apontador copiaLista(TipoLista *);
void imprimeLista(TipoLista *);
int profundidadeLista(TipoLista *);
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

        if (opcao== 1) {}
        else if (opcao== 2) {}
        else if (opcao== 3) {}
        else if (opcao== 4) {}
        else if (opcao== 5) printf("\nProfundidade da Lista: %d\n\n", profundidadeLista(&lista));
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

void pesquisa(TipoLista *L, tipoChave chave) {
    Apontador auxiliar;

    if (listaVazia(L)) printf("\nErro! Lista vazia!!!\n\n");
    else {
        auxiliar= buscaAtomo(L, chave);

        if (auxiliar== NULL) printf("\nElemento nao encontrado!!!\n\n");
        else {
            printf("\nChave: %d", auxiliar->item.carAtomo.chave);
            /* Demais campos do item... */
            printf("\n\n");
        }
    }
}

Apontador buscaAtomo(TipoLista *L, tipoChave chave) {




}

Apontador copiaLista(TipoLista *L) {




}

void imprimeLista(TipoLista *L) {




}

int profundidadeLista(TipoLista *L) {




}

void finalizaExecucao(TipoLista *L) {




}
