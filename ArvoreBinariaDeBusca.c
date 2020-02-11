/*   Algoritmos e Estruturas de Dados I - Árvores Binárias de Busca
              Evandro Scudeleti Ortigossa, n° USP 6793135
              Bacharelado em Ciências de Computação 2009           */

#include <stdio.h>
#include <stdlib.h>

typedef int tipoChave;

typedef struct {
    tipoChave chave;
    /* Demais campos do item... */
} tipoItem;

typedef struct NoArvore {
    tipoItem item;
    struct NoArvore *direita;
    struct NoArvore *esquerda;
} TipoNoArvore;

typedef TipoNoArvore *Apontador;
typedef Apontador TipoArvore;

void inicializaArvore(TipoArvore *);
int arvoreVazia(TipoArvore *);
void criaRaiz(TipoArvore *, tipoItem);
void insereArvore(TipoArvore *, tipoItem);
void buscaMenor(Apontador *, Apontador *);
void removeArvore(TipoArvore *, tipoChave);
void pesquisa(TipoArvore *, tipoChave);
Apontador buscaElemento(TipoArvore *, tipoChave);
void preOrdem(Apontador);
void emOrdem(Apontador);
void posOrdem(Apontador);
void visitaElemento(Apontador);
int alturaArvore(TipoArvore *);
void finalizaExecucao(TipoArvore *);

int main(void) {
    int opcao= 1, aux= 0;
    TipoArvore arvore;
    tipoItem item;

    inicializaArvore(&arvore);

    while(opcao!= 0) {
        scanf("%d", &opcao);
        if (opcao!= 0 && opcao< 4) scanf("%d", &aux);
        item.chave= aux;

        if (opcao== 1) insereArvore(&arvore, item);
        else if (opcao== 2) removeArvore(&arvore, aux);
        else if (opcao== 3) pesquisa(&arvore, aux);
        else if (opcao== 4) preOrdem(arvore);
        else if (opcao== 5) emOrdem(arvore);
        else if (opcao== 6) posOrdem(arvore);
        else if (opcao== 7) printf("\nAltura da Arvore: %d\n\n", alturaArvore(&arvore));

        if (opcao> 2 && opcao< 7) {
            if (opcao!= 3 && arvoreVazia(&arvore)) printf("\nErro! Arvore vazia!!!\n\n");
            else printf("\n\n");
        }
    }
    finalizaExecucao(&arvore);

return 0;

}

void inicializaArvore(TipoArvore *A) {
    (*A)= NULL;
}

int arvoreVazia(TipoArvore *A) {

    if ((*A)== NULL) return 1;
    return 0;
}

void criaRaiz(TipoArvore *A, tipoItem I) {
    (*A)= (Apontador)malloc(sizeof(TipoNoArvore));

    if ((*A)== NULL) printf("\nErro! Memoria indisponivel!!!\n\n");
    else {
        (*A)->item= I;
        (*A)->direita= NULL;           /* Alocação de memória para receber o elemento a ser inserido */
        (*A)->esquerda= NULL;
    }
}

void insereArvore(TipoArvore *A, tipoItem I) {
                                       /* Busca recursiva da posição adequada para inserção: elementos maiores que o "raiz" */
    if ((*A)== NULL) criaRaiz(A, I);   /* à direita, menores à esquerda */
    else {
        if (I.chave> (*A)->item.chave) insereArvore((&(*A)->direita), I);
        else if (I.chave< (*A)->item.chave) insereArvore((&(*A)->esquerda), I);
        else printf("\nErro! Chave existente!!!\n\n");
    }
}

void buscaMenor(Apontador *noRaiz, Apontador *substituto) {

    if ((*substituto)->esquerda!= NULL) buscaMenor(noRaiz, (&(*substituto)->esquerda));
    else {
        (*noRaiz)->item= (*substituto)->item;    /* Busca de elemento "folha" de menor valor na subárvore direita para */
        (*noRaiz)= *substituto;                  /* substituição na posição do elemento "raiz" que se quer remover. */
        (*substituto)= (*substituto)->direita;   /* Só há liberação de memória em folhas */
    }
}

void removeArvore(TipoArvore *A, tipoChave chave) {
    Apontador auxiliar;

    if ((*A)== NULL) printf("\nElemento nao encontrado!!!\n\n");
    else {                                       /* Busca recursiva pelo elemento a ser removido */
        if (chave> (*A)->item.chave) removeArvore((&(*A)->direita), chave);
        else if (chave< (*A)->item.chave) removeArvore((&(*A)->esquerda), chave);
        else {
            auxiliar= *A;

            if (auxiliar->direita== NULL) (*A)= auxiliar->esquerda;
            else if (auxiliar->esquerda== NULL) (*A)= auxiliar->direita;
            else buscaMenor(&auxiliar, &(auxiliar->direita));        /* Caso em que o elemento é "raiz", ou seja, possui */
                                                                     /* subárvore direita e subárvore esquerda */
            free(auxiliar);
        }
    }
}

void pesquisa(TipoArvore *A, tipoChave chave) {
    Apontador auxiliar;

    if (arvoreVazia(A)) printf("\nErro! Arvore vazia!!!");
    else {
        auxiliar= buscaElemento(A, chave);

        if (auxiliar== NULL) printf("\nElemento nao encontrado!!!");
        else visitaElemento(auxiliar);
    }
}

Apontador buscaElemento(TipoArvore *A, tipoChave chave) {
    Apontador auxiliar= *A;

    if ((*A)!= NULL) {
        if (chave> auxiliar->item.chave) auxiliar= buscaElemento((&(*A)->direita), chave);
        else if (chave< auxiliar->item.chave) auxiliar= buscaElemento((&(*A)->esquerda), chave);
    }                             /* Busca recursiva pelo elemento semelhante a da inserção e da remoção, no entanto, retorna */
    return auxiliar;              /* um ponteiro para o elemento, quando encontrado, ou NULL, quando não encontrado */
}

void preOrdem(Apontador A) {      /* Funções básicas de percurso em árvores binárias */

    if (A!= NULL) {
        visitaElemento(A);
        preOrdem(A->esquerda);    /* preOrdem visita o nó "raiz" e suas subárvores, listando os elementos da esquerda para a direita */
        preOrdem(A->direita);
    }
}

void emOrdem(Apontador A) {

    if (A!= NULL) {
        emOrdem(A->esquerda);
        visitaElemento(A);        /* emOrdem visita os elementos da árvore binária por ordem crescente de chaves */
        emOrdem(A->direita);
    }
}

void posOrdem(Apontador A) {

    if (A!= NULL) {
        posOrdem(A->esquerda);
        posOrdem(A->direita);     /* posOrdem visita os elementos das subárvores em níveis, da esquerda para a direita */
        visitaElemento(A);        /* Logo, a "raiz" da árvore é o último elemento a ser visitado */
    }
}

void visitaElemento(Apontador A) {

    printf("\nChave: %d", A->item.chave);
    /* Demais campos do item... */
}

int alturaArvore(TipoArvore *A) {
    int altura, altDireita, altEsquerda;

    if ((*A)== NULL) return 0;
    else {
        altDireita= alturaArvore(&(*A)->direita);          /* Ao chegar em um nó folha a função retorna zero, que vai sendo */
        altEsquerda= alturaArvore(&(*A)->esquerda);        /* incrementado a cada nível na volta recursiva da função */

        if (altDireita> altEsquerda) altura= altDireita;
        else altura= altEsquerda;

        return (altura+ 1);                      /* Altura de uma árvore binária é determinada pela altura (níveis) de sua */
    }                                            /* maior subárvore em relação a "raiz" */
}

void finalizaExecucao(TipoArvore *A) {

    if ((*A)!= NULL) {
        finalizaExecucao(&(*A)->esquerda);       /* Liberação da memória do tipo posOrdem: liberação dos elementos em níveis, */
        finalizaExecucao(&(*A)->direita);        /* da esquerda para a direita */
        free(*A);
        (*A)= NULL;
    }
}
