/*        Algoritmos e Estruturas de Dados I - Árvores AVL
            Evandro Scudeleti Ortigossa, n° USP 6793135
            Bacharelado em Ciências de Computação 2009         */

#include <stdio.h>
#include <stdlib.h>

typedef int tipoChave;

typedef struct {
    tipoChave chave;
    /* Demais campos do item... */
} tipoItem;

typedef struct NoArvoreAVL {
    tipoItem item;
    int balanceFactor;                           /* Fator de balanceamento do nó: -1, 0, +1 */
    struct NoArvoreAVL *direita;
    struct NoArvoreAVL *esquerda;
} TipoNoArvoreAVL;

typedef TipoNoArvoreAVL *Apontador;
typedef Apontador TipoArvoreAVL;

typedef enum {
    verdadeiro, falso
} boolean;

void inicializaArvore(TipoArvoreAVL *);
int arvoreVazia(TipoArvoreAVL *);
void criaRaiz(TipoArvoreAVL *, tipoItem, boolean *);
void rotacaoDireita(Apontador *, boolean);
void rotacaoEsquerda(Apontador *, boolean);
void rotacaoDireitaEsquerda(Apontador *, boolean);
void rotacaoEsquerdaDireita(Apontador *, boolean);
void insereArvoreAVL(TipoArvoreAVL *, tipoItem, boolean *);
void balanceDireita(Apontador *, boolean *);
void balanceEsquerda(Apontador *, boolean *);
void buscaMenor(Apontador *, Apontador *, boolean *);
void buscaMaior(Apontador *, Apontador *, boolean *);
void removeArvoreAVL(TipoArvoreAVL *, tipoChave, boolean *);
void pesquisa(TipoArvoreAVL *, tipoChave);
Apontador buscaElemento(TipoArvoreAVL *, tipoChave);
void preOrdem(Apontador);
void emOrdem(Apontador);
void posOrdem(Apontador);
void visitaElemento(Apontador);
int alturaArvore(TipoArvoreAVL *);
void finalizaExecucao(TipoArvoreAVL *);

int main(void) {
    int opcao= 1, aux= 0;
    TipoArvoreAVL arvore;
    tipoItem item;
    boolean flag;

    inicializaArvore(&arvore);

    while(opcao!= 0) {
        scanf("%d", &opcao);
        if (opcao!= 0 && opcao< 4) scanf("%d", &aux);
        item.chave= aux;
        flag= falso;

        if (opcao== 1) insereArvoreAVL(&arvore, item, &flag);
        else if (opcao== 2) removeArvoreAVL(&arvore, aux, &flag);
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

void inicializaArvore(TipoArvoreAVL *A) {
    (*A)= NULL;
}

int arvoreVazia(TipoArvoreAVL *A) {

    if ((*A)== NULL) return 1;
    return 0;
}

void criaRaiz(TipoArvoreAVL *A, tipoItem I, boolean *flag) {
    (*A)= (Apontador)malloc(sizeof(TipoNoArvoreAVL));

    if ((*A)== NULL) printf("\nErro! Memoria indisponivel!!!\n\n");
    else {
        (*A)->item= I;
        (*A)->balanceFactor= 0;
        (*A)->direita= NULL;                     /* Alocação de memória para receber o elemento a ser inserido */
        (*A)->esquerda= NULL;
        (*flag)= verdadeiro;
    }
}

void rotacaoDireita(Apontador *pA, boolean flag) {
    Apontador pB;

    pB= (*pA)->direita;
    (*pA)->direita= pB->esquerda;
    pB->esquerda= *pA;                           /* Área comum do algoritmo de rotação (Inserção/Exclusão) */
    (*pA)->balanceFactor= 0;

    if (flag== falso) {
        if (pB->balanceFactor== 0) {
            (*pA)->balanceFactor= -1;            /* "flag" falso indica rotação Direita na exclusão */
            pB->balanceFactor= 1;
        }
        else pB->balanceFactor= 0;
    }
    (*pA)= pB;
}

void rotacaoEsquerda(Apontador *pA, boolean flag) {
    Apontador pB;

    pB= (*pA)->esquerda;
    (*pA)->esquerda= pB->direita;                /* Após uma rotação Direita ou Esquerda, os nós A e B passam a ter fator de */
    pB->direita= *pA;                            /* balanceamento iguais a zero na inserção */
    (*pA)->balanceFactor= 0;

    if (flag== falso) {
        if (pB->balanceFactor== 0) {             /* Após uma rotação Direita ou Esquerda na remoção, fator de balanceamento dos */
            (*pA)->balanceFactor= 1;             /* nós A e B podem variar */
            pB->balanceFactor= -1;
        }
        else pB->balanceFactor= 0;
    }
    (*pA)= pB;
}

void rotacaoDireitaEsquerda(Apontador *pA, boolean flag) {
    Apontador pB, pC;

    pB= (*pA)->direita;
    pC= pB->esquerda;
    pB->esquerda= pC->direita;                   /* Área comum do algoritmo de rotação (Inserção/Exclusão) */
    pC->direita= pB;
    (*pA)->direita= pC->esquerda;
    pC->esquerda= *pA;

    if (pC->balanceFactor== -1) (*pA)->balanceFactor= 1;
    else (*pA)->balanceFactor= 0;

    if (pC->balanceFactor== 1) pB->balanceFactor= -1;
    else pB->balanceFactor= 0;
    (*pA)= pC;

    if (flag== falso) pC->balanceFactor= 0;      /* "flag" falso indica rotação Direita-Esquerda na exclusão */
}

void rotacaoEsquerdaDireita(Apontador *pA, boolean flag) {
    Apontador pB, pC;

    pB= (*pA)->esquerda;
    pC= pB->direita;                             /* No caso de rotações Direita-Esquerda ou Esquerda-Direita os fatores de balanceamento */
    pB->direita= pC->esquerda;                   /* dos nós A e B serão recalculados com base no fator de balanceamento do nó C */
    pC->esquerda= pB;
    (*pA)->esquerda= pC->direita;
    pC->direita= *pA;

    if (pC->balanceFactor== 1) (*pA)->balanceFactor= -1;
    else (*pA)->balanceFactor= 0;

    if (pC->balanceFactor== -1) pB->balanceFactor= 1;
    else pB->balanceFactor= 0;
    (*pA)= pC;

    if (flag== falso) pC->balanceFactor= 0;      /* "flag" falso indica rotação Esquerda-Direita na exclusão */
}

void insereArvoreAVL(TipoArvoreAVL *A, tipoItem I, boolean *flag) {
                                                 /* Busca recursiva da posição adequada para inserção: elementos maiores que o "raiz" */
    if ((*A)== NULL) criaRaiz(A, I, flag);       /* à direita, menores à esquerda */
    else {
        if (I.chave> (*A)->item.chave) {
            insereArvoreAVL((&(*A)->direita), I, flag);
            if ((*flag)== verdadeiro) {          /** Subárvore direita cresceu **/
                if ((*A)->balanceFactor== 1) {
                    (*A)->balanceFactor= 0;
                    (*flag)= falso;
                }
                else if ((*A)->balanceFactor== 0) (*A)->balanceFactor= -1;
                else if ((*A)->balanceFactor== -1) {
                    if ((*A)->direita->balanceFactor== -1) rotacaoDireita(A, verdadeiro);
                    else rotacaoDireitaEsquerda(A, verdadeiro);
                    (*A)->balanceFactor= 0;
                    (*flag)= falso;
                }                                /* O parâmetro "flag", passado por referência (com valor inicial falso), indica que a */
            }                                    /* subárvore cresceu em altura */
        }
        else if (I.chave< (*A)->item.chave) {
            insereArvoreAVL((&(*A)->esquerda), I, flag);
            if ((*flag)== verdadeiro) {          /** Subárvore esquerda cresceu **/
                if ((*A)->balanceFactor== -1) {
                    (*A)->balanceFactor= 0;
                    (*flag)= falso;
                }
                else if ((*A)->balanceFactor== 0) (*A)->balanceFactor= 1;
                else if ((*A)->balanceFactor== 1) {
                    if ((*A)->esquerda->balanceFactor== 1) rotacaoEsquerda(A, verdadeiro);
                    else rotacaoEsquerdaDireita(A, verdadeiro);
                    (*A)->balanceFactor= 0;
                    (*flag)= falso;
                }
            }
        }
        else printf("\nErro! Chave existente!!!\n\n");
    }
}

void balanceDireita(Apontador *pA, boolean *flag) {
    int balance= (*pA)->balanceFactor;

    if (balance== -1) (*pA)->balanceFactor= 0;   /** Subárvore direita encolheu **/
    else if (balance== 0) {
        (*pA)->balanceFactor= 1;
        (*flag)= falso;
    }
    else if (balance== 1) {
        balance= (*pA)->esquerda->balanceFactor;

        if (balance>= 0) {
            rotacaoEsquerda(pA, falso);
            if (balance== 0) (*flag)= falso;
        }
        else rotacaoEsquerdaDireita(pA, falso);
    }
}

void balanceEsquerda(Apontador *pA, boolean *flag) {
    int balance= (*pA)->balanceFactor;

    if (balance== 1) (*pA)->balanceFactor= 0;    /** Subárvore esquerda encolheu **/
    else if (balance== 0) {
        (*pA)->balanceFactor= -1;
        (*flag)= falso;
    }
    else if (balance== -1) {
        balance= (*pA)->direita->balanceFactor;

        if (balance<= 0) {
            rotacaoDireita(pA, falso);
            if (balance== 0) (*flag)= falso;
        }
        else rotacaoDireitaEsquerda(pA, falso);
    }
}

void buscaMenor(Apontador *noRaiz, Apontador *substituto, boolean *flag) {

    if ((*substituto)->esquerda!= NULL) {
        buscaMenor(noRaiz, (&(*substituto)->esquerda), flag);
        if ((*flag)== verdadeiro) balanceEsquerda(substituto, flag);
    }
    else {
        (*noRaiz)->item= (*substituto)->item;    /* Busca de elemento "folha" de menor valor na subárvore direita para */
        (*noRaiz)= *substituto;                  /* substituição na posição do elemento "raiz" que se quer remover. */
        (*substituto)= (*substituto)->direita;   /* Só há liberação de memória em folhas */
        (*flag)= verdadeiro;
    }
}

void buscaMaior(Apontador *noRaiz, Apontador *substituto, boolean *flag) {

    if ((*substituto)->direita!= NULL) {
        buscaMenor(noRaiz, (&(*substituto)->direita), flag);
        if ((*flag)== verdadeiro) balanceDireita(substituto, flag);
    }
    else {
        (*noRaiz)->item= (*substituto)->item;    /* Busca de elemento "folha" de maior valor na subárvore esquerda para */
        (*noRaiz)= *substituto;                  /* substituição na posição do elemento "raiz" que se quer remover. */
        (*substituto)= (*substituto)->esquerda;
        (*flag)= verdadeiro;
    }
}

void removeArvoreAVL(TipoArvoreAVL *A, tipoChave chave, boolean *flag) {
    Apontador auxiliar;                          /* A remoção em Árvores AVL é similar à remoção em uma Árvore Binária de Busca, no entanto, */
                                                 /* é preciso verificar o balanceamento e, se necessário, aplicar algumas das rotações */
    if ((*A)== NULL) printf("\nElemento nao encontrado!!!\n\n");
    else {
        if (chave> (*A)->item.chave) {
            removeArvoreAVL((&(*A)->direita), chave, flag);
            if ((*flag)== verdadeiro) balanceDireita(A, flag);
        }
        else if (chave< (*A)->item.chave) {
            removeArvoreAVL((&(*A)->esquerda), chave, flag);
            if ((*flag)== verdadeiro) balanceEsquerda(A, flag);
        }
        else {
            auxiliar= *A;                        /* O parâmetro "flag", passado por referência (cujo valor inicial deve ser falso) indica */
                                                 /* que a altura da subárvore foi reduzida */
            if (auxiliar->direita== NULL) {
                (*A)= auxiliar->esquerda;
                (*flag)= verdadeiro;             /* Os algoritmos de rebalanceamento somente são efetuados se "flag" for verdadeiro */
            }
            else if (auxiliar->esquerda== NULL) {
                (*A)= auxiliar->direita;
                (*flag)= verdadeiro;
            }
            else {
                if ((auxiliar->balanceFactor)< 0) {
                    buscaMaior(&auxiliar, &(auxiliar->esquerda), flag);
                    if ((*flag)== verdadeiro) balanceEsquerda(A, flag);
                }                                                         /* Caso em que o elemento é "raiz", ou seja, possui subárvore */
                else {                                                    /* direita e subárvore esquerda */
                    buscaMenor(&auxiliar, &(auxiliar->direita), flag);
                    if ((*flag)== verdadeiro) balanceDireita(A, flag);
                }
            }
            free(auxiliar);
        }
    }
}

void pesquisa(TipoArvoreAVL *A, tipoChave chave) {
    Apontador auxiliar;

    if (arvoreVazia(A)) printf("\nErro! Arvore vazia!!!");
    else {
        auxiliar= buscaElemento(A, chave);

        if (auxiliar== NULL) printf("\nElemento nao encontrado!!!");
        else visitaElemento(auxiliar);
    }
}

Apontador buscaElemento(TipoArvoreAVL *A, tipoChave chave) {
    Apontador auxiliar= *A;

    if ((*A)!= NULL) {
        if (chave> auxiliar->item.chave) auxiliar= buscaElemento((&(*A)->direita), chave);
        else if (chave< auxiliar->item.chave) auxiliar= buscaElemento((&(*A)->esquerda), chave);
    }                                            /* Busca recursiva pelo elemento semelhante a da inserção e da remoção, no entanto, retorna */
    return auxiliar;                             /* um ponteiro para o elemento, quando encontrado, ou NULL, quando não encontrado */
}

void preOrdem(Apontador A) {                     /* Funções básicas de percurso em árvores binárias */

    if (A!= NULL) {
        visitaElemento(A);
        preOrdem(A->esquerda);         /* preOrdem visita o nó "raiz" e suas subárvores, listando os elementos da esquerda para a direita */
        preOrdem(A->direita);
    }
}

void emOrdem(Apontador A) {

    if (A!= NULL) {
        emOrdem(A->esquerda);
        visitaElemento(A);             /* emOrdem visita os elementos da árvore binária por ordem crescente de chaves */
        emOrdem(A->direita);
    }
}

void posOrdem(Apontador A) {

    if (A!= NULL) {
        posOrdem(A->esquerda);
        posOrdem(A->direita);          /* posOrdem visita os elementos das subárvores em níveis, da esquerda para a direita */
        visitaElemento(A);             /* Logo, a "raiz" da árvore é o último elemento a ser visitado */
    }
}

void visitaElemento(Apontador A) {

    printf("\nChave: %d", A->item.chave);
    /* Demais campos do item... */
}

int alturaArvore(TipoArvoreAVL *A) {
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

void finalizaExecucao(TipoArvoreAVL *A) {

    if ((*A)!= NULL) {
        finalizaExecucao(&(*A)->esquerda);       /* Liberação da memória do tipo posOrdem: liberação dos elementos em níveis, */
        finalizaExecucao(&(*A)->direita);        /* da esquerda para a direita */
        free(*A);
        (*A)= NULL;
    }
}
