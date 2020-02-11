/*      Algoritmos e Estruturas de Dados I - Matrizes Esparsas
             Evandro Scudeleti Ortigossa, n° USP 6793135
             Bacharelado em Ciências de Computação 2009        */

#include <stdio.h>
#include <stdlib.h>

#define MAXLIN 39
#define MAXCOL 39

typedef int tipoChave;
typedef int tipoPosicao;

typedef struct {
    tipoChave chave;
    /* Demais campos do item... */
} tipoItem;

typedef struct NoMatriz {
    tipoPosicao linha;
    tipoPosicao coluna;
    tipoItem item;
    struct NoMatriz *antLinha;
    struct NoMatriz *antColuna;
    struct NoMatriz *proxLinha;
    struct NoMatriz *proxColuna;
} TipoNoMatriz;

typedef TipoNoMatriz *Apontador;

typedef struct {
    Apontador linhas[MAXLIN];          /* Matriz Esparsa constitui de duas listas duplamente encadeadas, uma para "Linhas" e outra para */
    Apontador colunas[MAXCOL];         /* "Colunas", cruzadas entre si */
    int contador;
} TipoMatriz;

void inicializaMatriz(TipoMatriz *);
int matrizVazia(TipoMatriz *);
int matrizCheia(TipoMatriz *);
void insereMatriz(TipoMatriz *, tipoPosicao, tipoPosicao, tipoItem);
void removeMatriz(TipoMatriz *, tipoPosicao, tipoPosicao, tipoItem *);
void pesquisa(TipoMatriz *, tipoPosicao, tipoPosicao);
Apontador buscaElemento(TipoMatriz *, tipoPosicao, tipoPosicao);
void imprimeMatriz(TipoMatriz *);
void somaConstante(TipoMatriz *, tipoItem);
void finalizaExecucao(TipoMatriz *);

int main(void) {
    int opcao= 1, aux= 0;
    tipoPosicao posL= 1, posC= 1;
    TipoMatriz matriz;
    tipoItem item;

    inicializaMatriz(&matriz);

    while(opcao!= 0) {
        scanf("%d", &opcao);
        if (opcao!= 0 && opcao< 4) scanf("%d %d", &posL, &posC);
        if (opcao== 1 || opcao== 4) scanf("%d", &aux);
        item.chave= aux;
                                       /* Funções exigem verificação anterior de posições válidas */
        if ((posL< 1) || (posL> MAXLIN) || (posC< 1) || (posC> MAXCOL)) printf("\n\nErro! Posicao invalida!!!\n\n");
        else {
            if (opcao== 1) insereMatriz(&matriz, (posL-1), (posC-1), item);
            else if (opcao== 2) removeMatriz(&matriz, (posL-1), (posC-1), &item);
            else if (opcao== 3) pesquisa(&matriz, (posL-1), (posC-1));
            else if (opcao== 4) somaConstante(&matriz, item);
            else if (opcao== 5) imprimeMatriz(&matriz);
        }
    }
    finalizaExecucao(&matriz);

return 0;

}

void inicializaMatriz(TipoMatriz *M) {
    int i;

    for ((i= 0); (i< MAXLIN); (i++)) M->linhas[i]= NULL;
    for ((i= 0); (i< MAXCOL); (i++)) M->colunas[i]= NULL;
    M->contador= 0;
}

int matrizVazia(TipoMatriz *M) {
    int i;

    for ((i= 0); (i< MAXLIN); (i++)) if (M->linhas[i]!= NULL) return 0;
    return 1;
}

int matrizCheia(TipoMatriz *M) {

    if (M->contador== MAXLIN*MAXCOL) return 1;
    return 0;
}

void insereMatriz(TipoMatriz *M, tipoPosicao posLinha, tipoPosicao posColuna, tipoItem I) {
    tipoPosicao posInsercao;
    Apontador auxiliar, posicao;

    if (matrizCheia(M)) printf("\nErro! Matriz cheia!!!\n\n");
    else {
        auxiliar= buscaElemento(M, posLinha, posColuna);

        if (auxiliar!= NULL) printf("\nErro! Elemento existente!!!\n\n");
        else {
            auxiliar= (Apontador)malloc(sizeof(TipoNoMatriz));

            if (auxiliar== NULL) printf("\nErro! Memoria indisponivel!!!\n\n");
            else {
                auxiliar->linha= posLinha;
                auxiliar->coluna= posColuna;
                auxiliar->item= I;

                posicao= M->linhas[posLinha];
                posInsercao= posColuna;

                if (posicao== NULL) {                      /** Inserção do elemento na lista de linhas **/
                    M->linhas[posLinha]= auxiliar;
                    auxiliar->antColuna= M->linhas[posLinha];
                    auxiliar->proxColuna= posicao;         /* Inserção do primeiro elemento da lista de linhas */
                }
                else if ((posicao->coluna)> posColuna) {
                    M->linhas[posLinha]= auxiliar;         /* Inserção na primeira posição da lista de linhas */
                    auxiliar->antColuna= M->linhas[posLinha];
                    auxiliar->proxColuna= posicao;
                    posicao->antColuna= auxiliar;
                }
                else {                                     /* Demais casos de inserção... */
                    while((posicao->coluna)< posInsercao) {
                        if (posicao->proxColuna!= NULL) {
                            if ((posicao->proxColuna->coluna)< posInsercao) posicao= posicao->proxColuna;
                            else posInsercao= -1;          /* Busca da posição ideal para inserção na lista de linhas, ordenação por coluna */
                        }
                        else posInsercao= -1;
                    }
                    auxiliar->antColuna= posicao;
                    auxiliar->proxColuna= posicao->proxColuna;
                    if (posicao->proxColuna!= NULL) posicao->proxColuna->antColuna= auxiliar;
                    posicao->proxColuna= auxiliar;         /* Inserção em qualquer outra posição da lista de linhas */
                }
                posicao= M->colunas[posColuna];
                posInsercao= posLinha;

                if (posicao== NULL) {                      /** Inserção do elemento na lista de colunas **/
                    M->colunas[posColuna]= auxiliar;
                    auxiliar->antLinha= M->colunas[posColuna];
                    auxiliar->proxLinha= posicao;          /* Inserção do primeiro elemento da lista de colunas */
                }
                else if ((posicao->linha)> posLinha) {
                    M->colunas[posColuna]= auxiliar;       /* Inserção na primeira posição da lista de colunas */
                    auxiliar->antLinha= M->colunas[posColuna];
                    auxiliar->proxLinha= posicao;
                    posicao->antLinha= auxiliar;
                }
                else {                                     /* Demais casos de inserção... */
                    while((posicao->linha)< posInsercao) {
                        if (posicao->proxLinha!= NULL) {
                            if ((posicao->proxLinha->linha)< posInsercao) posicao= posicao->proxLinha;
                            else posInsercao= -1;          /* Busca da posição ideal para inserção na lista de colunas, ordenação por linha */
                        }
                        else posInsercao= -1;
                    }
                    auxiliar->antLinha= posicao;
                    auxiliar->proxLinha= posicao->proxLinha;
                    if (posicao->proxLinha!= NULL) posicao->proxLinha->antLinha= auxiliar;
                    posicao->proxLinha= auxiliar;          /* Inserção em qualquer outra posição da lista de colunas */
                }
                M->contador++;
            }
        }
    }
}

void removeMatriz(TipoMatriz *M, tipoPosicao posLinha, tipoPosicao posColuna, tipoItem *I) {
    Apontador auxiliar;

    if (matrizVazia(M)) printf("\nErro! Matriz vazia!!!\n\n");
    else {
        auxiliar= buscaElemento(M, posLinha, posColuna);

        if (auxiliar== NULL) printf("\nErro! Elemento nao encontrado!!!\n\n");
        else {
            if (auxiliar== M->linhas[posLinha]) {               /* Remoção do primeiro elemento da lista de linhas */
                M->linhas[posLinha]= auxiliar->proxColuna;
                if (M->linhas[posLinha]!= NULL) auxiliar->proxColuna->antColuna= M->linhas[posLinha];
            }                                                   /* Caso em que o elemento é único na lista de linhas */
            else {                                              /* Remoção de elemento qualquer da lista de linhas */
                if (auxiliar->proxColuna!= NULL) auxiliar->proxColuna->antColuna= auxiliar->antColuna;
                auxiliar->antColuna->proxColuna= auxiliar->proxColuna;
            }

            if (auxiliar== M->colunas[posColuna]) {             /* Remoção do primeiro elemento da lista de colunas */
                M->colunas[posColuna]= auxiliar->proxLinha;
                if (M->colunas[posColuna]!= NULL) auxiliar->proxLinha->antLinha= M->colunas[posColuna];
            }                                                   /* Caso em que o elemento é único na lista de colunas */
            else {                                              /* Remoção de elemento qualquer da lista de colunas */
                if (auxiliar->proxLinha!= NULL) auxiliar->proxLinha->antLinha= auxiliar->antLinha;
                auxiliar->antLinha->proxLinha= auxiliar->proxLinha;
            }
            M->contador--;
            free(auxiliar);
        }
    }
}

void pesquisa(TipoMatriz *M, tipoPosicao posLinha, tipoPosicao posColuna) {
    Apontador auxiliar;

    if (matrizVazia(M)) printf("\nErro! Matriz vazia!!!\n\n");
    else {
        auxiliar= buscaElemento(M, posLinha, posColuna);

        if (auxiliar== NULL) printf("\nElemento nao encontrado!!!\n\n");
        else {
            printf("\nChave: %d", auxiliar->item.chave);
            /* Demais campos do item... */
            printf("\n\n");
        }
    }
}

Apontador buscaElemento(TipoMatriz *M, tipoPosicao posLinha, tipoPosicao posColuna) {
    Apontador auxiliar= M->linhas[posLinha];

    if (auxiliar!= NULL) {
        while((auxiliar->coluna)< posColuna) {
            auxiliar= auxiliar->proxColuna;

            if (auxiliar== NULL) return NULL;
        }
        if (auxiliar->coluna== posColuna) return auxiliar;
    }
    return NULL;
}

void imprimeMatriz(TipoMatriz *M) {
    int i, j, k, coluna;
    Apontador auxiliar;

    for ((i= 0); (i< MAXLIN); (i++)) {                     /* Impressão linha a linha */
        auxiliar= M->linhas[i];
        k= 0, coluna= 0;
        printf("\n");

        while(coluna< MAXCOL) {                            /* Garantia de impressão de todas as colunas da linha atual */
            if (auxiliar!= NULL) coluna= auxiliar->coluna;
            else coluna= MAXCOL;                           /* Ponto de parada da impressão da linha, marca a posição de coluna do elemento não nulo */

            for ((j= k); (j< coluna); (j++)) printf("- "); /* Impressão até o elemento não nulo */
            if (auxiliar!= NULL) {
                printf("%d ", auxiliar->item.chave);       /* Impressão do elemento não nulo */
                /* Demais campos do item... */
            }
            k= (coluna +1);                                /* Parâmetro para continuar a impressão da linha a partir do elemento não nulo */
            if (auxiliar!= NULL) auxiliar= auxiliar->proxColuna;
        }
    }
    printf("\n\n");
}

void somaConstante(TipoMatriz *M, tipoItem constItem) {
    int i, j;
    Apontador auxiliar;
    tipoItem auxItem;

    if ((constItem.chave)!= 0) {                           /* Só considera-se operações com valores não nulos */
        for ((i= 0); (i< MAXLIN); (i++)) {
            for ((j= 0); (j< MAXCOL); (j++)) {
                auxiliar= buscaElemento(M, i, j);          /* Busca do elemento por posição na matriz */
                                                           /* Caso o elemento não exista (zero), é criado com o valor da constante */
                if (auxiliar== NULL) insereMatriz(M, i, j, constItem);
                else {                                     /* Caso exista, é feita a soma da constante com o valor atual do elemento */
                    auxiliar->item.chave += constItem.chave;
                    /* Demais campos do item... */
                                                           /* Se a operação resultar em um elemento nulo, exclui-se o mesmo */
                    if ((auxiliar->item.chave)== 0) removeMatriz(M, i, j, &auxItem);
                }
            }
        }
    }
}

void finalizaExecucao(TipoMatriz *M) {
    int i;
    tipoItem item;

    for ((i= 0); (i< MAXLIN); (i++)) {
        while(M->linhas[i]!= NULL) removeMatriz(M, (M->linhas[i]->linha), (M->linhas[i]->coluna), &item);
    }
}
