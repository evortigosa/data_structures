/* Algoritmos e Estruturas de Dados II - Trabalho 1 - Grafos
        Evandro Scudeleti Ortigossa, n° USP 6793135
       Bacharelado em Ciências de Computação           */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAXDIST INT_MAX

typedef int identificador;
typedef int tipoPeso;
typedef unsigned int tipoDistancia;

typedef struct idNoVertex {
    identificador id;
    struct idNoVertex *anterior;
    struct idNoVertex *proximo;
    struct NoVertice *vertice;
} TipoIdVertex;

typedef struct idNoAresta {
    identificador id;
    struct idNoAresta *anterior;
    struct idNoAresta *proximo;
    struct NoAresta *aresta;
} TipoIdAresta;

typedef struct NoVertice {
    tipoPeso pVertice;
    tipoDistancia distancia;
    struct NoVertice *anterior;
    struct NoVertice *proximo;
    struct NoVertice *pai;
    struct NoAuxiliar *LArestas;
    struct idNoVertex *idV;
} TipoNoVertice;

typedef struct NoAresta {
    tipoPeso pAresta;
    struct NoAresta *anterior;
    struct NoAresta *proximo;
    struct NoVertice *vertexA;
    struct NoVertice *vertexB;
    struct NoAuxiliar *adjVertexA;
    struct NoAuxiliar *adjVertexB;
    struct idNoAresta *idA;
} TipoNoAresta;

typedef struct NoAuxiliar {
    struct NoAuxiliar *anterior;
    struct NoAuxiliar *proximo;
    struct NoAresta *aresta;
} TipoNoAuxiliar;

typedef TipoIdVertex *ApontadorIdV;
typedef TipoIdAresta *ApontadorIdA;
typedef TipoNoVertice *ApontadorVertex;
typedef TipoNoAresta *ApontadorArestas;
typedef TipoNoAuxiliar *ApontadorAux;

typedef ApontadorIdV ListaIdVertex;
typedef ApontadorIdA ListaIdArestas;

typedef struct {
    ApontadorVertex vertices;
    ApontadorArestas arestas;
    int numVertices;
    int numArestas;
} TipoGrafo;

typedef TipoGrafo *ApontadorGrafo;
typedef ApontadorGrafo Grafo;

typedef struct {
    struct NoVertice *vertice;
} tipoItem;

typedef struct NoFila {
    tipoItem item;
    struct NoFila *anterior;
    struct NoFila *proximo;
} TipoNoFila;

typedef TipoNoFila *ApontadorFila;

typedef struct {
    ApontadorFila primeiro;
    ApontadorFila ultimo;
} TipoFila;

void inicializaEstruturas(Grafo *, ListaIdVertex *, ListaIdArestas *);
void processaComando(int *, identificador *, identificador *, tipoPeso *);
ApontadorVertex insereVertice(Grafo *, tipoPeso, ListaIdVertex *, identificador);
tipoPeso deletaVertice(Grafo *, ListaIdVertex *, identificador, ListaIdArestas *);
void alteraValorVertice(Grafo *, ListaIdVertex *, identificador, tipoPeso);
tipoPeso valorVertice(ApontadorVertex, identificador *);
int saoAdjacentes(ApontadorVertex, ApontadorVertex);
ApontadorVertex verticeOposto(ApontadorVertex, ApontadorArestas);
void insereEstruturaAdj(ApontadorVertex, ApontadorArestas, ApontadorAux);
ApontadorArestas insereAresta(Grafo *, tipoPeso, ListaIdArestas *, identificador, ListaIdVertex *, identificador, identificador);
void deletaEstruturaAdj(ApontadorVertex, ApontadorAux);
tipoPeso deletaAresta(Grafo *, ListaIdArestas *, identificador);
void alteraValorAresta(Grafo *, ListaIdArestas *, identificador, tipoPeso);
tipoPeso valorAresta(ApontadorArestas, identificador *, identificador *, identificador *);
void verticesFinais(ApontadorArestas, ApontadorVertex *, ApontadorVertex *);
void imprimeGrafo(Grafo *, ListaIdVertex *, ListaIdArestas *);
void inicializaFila(TipoFila *);
int filaVazia(TipoFila *);
void enfileira(TipoFila *, tipoItem);
void desenfileira(TipoFila *, tipoItem *);
void reorganizaFila(TipoFila *, ApontadorVertex);
void caminhoMinimo(Grafo *, ApontadorVertex, identificador, identificador, int);
ApontadorVertex dijkstra(Grafo *, identificador, identificador);
void finalizaGrafo(Grafo *, ListaIdVertex *, ListaIdArestas *);

int main(void) {
    int comando= 0, flag= 0;
    identificador identificaA, identificaB, indexV= 0, indexA= 0;
    tipoPeso valorInsercao;
    Grafo grafoVA;
    ListaIdVertex idVertices;
    ListaIdArestas idArestas;
    ApontadorIdV v, x;
    ApontadorIdA g;
    ApontadorArestas k;
    ApontadorVertex z, w;

    inicializaEstruturas(&grafoVA, &idVertices, &idArestas);

    while(comando< 12) {
        comando= 0;
        processaComando(&comando, &identificaA, &identificaB, &valorInsercao);

        if (comando== 1) {
            indexV++;
            insereVertice(&grafoVA, valorInsercao, &idVertices, indexV);
        }
        else if (comando== 2) deletaVertice(&grafoVA, &idVertices, identificaA, &idArestas);
        else if (comando== 3) {
            indexA++;
            insereAresta(&grafoVA, valorInsercao, &idArestas, indexA, &idVertices, identificaA, identificaB);
        }
        else if (comando== 4) deletaAresta(&grafoVA, &idArestas, identificaA);
        else if (comando== 5) alteraValorVertice(&grafoVA, &idVertices, identificaA, valorInsercao);
        else if (comando== 6) alteraValorAresta(&grafoVA, &idArestas, identificaA, valorInsercao);
        else if (comando== 7) caminhoMinimo(&grafoVA, grafoVA->vertices, identificaA, identificaB, 0);
        else if (comando== 8) imprimeGrafo(&grafoVA, &idVertices, &idArestas);
        else if (comando== 9) {
            v= idVertices;
            x= idVertices;
            while (v->id!= identificaA) v= v->proximo;
            while (x->id!= identificaB) x= x->proximo;

            if (saoAdjacentes(v->vertice, x->vertice)) printf("\nSAO ADJACENTEEEEEEEES!!!\n");
            else printf("\nNAAAAO ADJACENTEEEEES!!!\n");
        }
        else if (comando== 10) {
            k= grafoVA->arestas;
            while (k->idA->id!= identificaA) k= k->proximo;

            verticesFinais(k, &z, &w);
            printf("\n%d %d\n", (z->idV->id), (w->idV->id));
        }
        else if (comando== 11) {
            v= idVertices;
            g= idArestas;
            while (v->id!= identificaA) v= v->proximo;
            while (g->id!= identificaB) g= g->proximo;

            w= verticeOposto(v->vertice, g->aresta);
            printf("\n%d\n", (w->idV->id));
        }
    }
    finalizaGrafo(&grafoVA, &idVertices, &idArestas);

return 0;

}

void inicializaEstruturas(Grafo *G, ListaIdVertex *ID1, ListaIdArestas *ID2) {
    ApontadorGrafo auxGrafo;

    auxGrafo= (ApontadorGrafo)malloc(sizeof(TipoGrafo));
    auxGrafo->vertices= NULL;
    auxGrafo->arestas= NULL;
    auxGrafo->numVertices= 0;
    auxGrafo->numArestas= 0;

    *G = auxGrafo;
    *ID1 = NULL;
    *ID2 = NULL;
}

void processaComando(int *opcao, identificador *idA, identificador *idB, tipoPeso *valorX) {
    char comando[50]= " ", auxChar[5]= " ";
    int ajuste, auxInt;

    scanf("\n");
    gets(comando);
    sscanf(comando, "%[^ ]s", auxChar);

    if (strcasecmp(auxChar, "CV")==0) {
        *opcao= 1;
        sscanf(&(comando[3]), "%d", valorX);
    }
    else if (strcasecmp(auxChar, "DV")==0) {
        *opcao= 2;
        sscanf(&(comando[3]), "%d", idA);
    }
    else if (strcasecmp(auxChar, "CA")==0) {
        *opcao= 3;
        sscanf(&(comando[3]), "%d %n", idA, &ajuste);
        sscanf(&(comando[3 + ajuste]), "%d %n", idB, &auxInt);
        sscanf(&(comando[3 + ajuste + auxInt]), "%d", valorX);
    }
    else if (strcasecmp(auxChar, "DA")==0) {
        *opcao= 4;
        sscanf(&(comando[3]), "%d", idA);
    }
    else if (strcasecmp(auxChar, "TV")==0) {
        *opcao= 5;
        sscanf(&(comando[3]), "%d %n", idA, &ajuste);
        sscanf(&(comando[3 + ajuste]), "%d", valorX);
    }
    else if (strcasecmp(auxChar, "TA")==0) {
        *opcao= 6;
        sscanf(&(comando[3]), "%d %n", idA, &ajuste);
        sscanf(&(comando[3 + ajuste]), "%d", valorX);
    }
    else if (strcasecmp(auxChar, "CM")==0) {
        *opcao= 7;
        sscanf(&(comando[3]), "%d %n", idA, &ajuste);
        sscanf(&(comando[3 + ajuste]), "%d", idB);
    }
    else if (strcasecmp(auxChar, "IG")==0) *opcao= 8;
    else if (strcasecmp(auxChar, "SA")==0) {
        *opcao= 9;
        sscanf(&(comando[3]), "%d %n", idA, &ajuste);
        sscanf(&(comando[3 + ajuste]), "%d", idB);
    }
    else if (strcasecmp(auxChar, "VF")==0) {
        *opcao= 10;
        sscanf(&(comando[3]), "%d", idA);
    }
    else if (strcasecmp(auxChar, "OP")==0) {
        *opcao= 11;
        sscanf(&(comando[3]), "%d %n", idA, &ajuste);
        sscanf(&(comando[3 + ajuste]), "%d", idB);
    }
    else if (strcasecmp(auxChar, "FM")==0) *opcao= 13;
}

ApontadorVertex insereVertice(Grafo *G, tipoPeso valorVertex, ListaIdVertex *ID, identificador valorID) {
    ApontadorVertex auxVertex;
    ApontadorIdV auxIdVertex;

    auxVertex= (ApontadorVertex)malloc(sizeof(TipoNoVertice));
    auxIdVertex= (ApontadorIdV)malloc(sizeof(TipoIdVertex));

    if ((*G)->vertices!= NULL) (*G)->vertices->anterior= auxVertex;
    auxVertex->pVertice= valorVertex;
    auxVertex->anterior= NULL;
    auxVertex->proximo= (*G)->vertices;
    auxVertex->pai= NULL;
    auxVertex->LArestas= NULL;
    auxVertex->idV= auxIdVertex;
    (*G)->numVertices++;

    if ((*ID)!= NULL) (*ID)->anterior= auxIdVertex;
    auxIdVertex->id= valorID;
    auxIdVertex->anterior= NULL;
    auxIdVertex->proximo= (*ID);
    auxIdVertex->vertice= auxVertex;

    (*G)->vertices= auxVertex;
    (*ID)= auxIdVertex;

    return (*G)->vertices;
}

tipoPeso deletaVertice(Grafo *G, ListaIdVertex *ID, identificador valorID, ListaIdArestas *IDare) {
    ApontadorVertex auxVertex;
    ApontadorIdV auxIdVertex= *ID;
    tipoPeso pVertice;

    while(auxIdVertex->id!= valorID) auxIdVertex= auxIdVertex->proximo;
    auxVertex= auxIdVertex->vertice;
    pVertice= auxVertex->pVertice;

    while(auxVertex->LArestas!= NULL) deletaAresta(G, IDare, (auxVertex->LArestas->aresta->idA->id));

    if (auxVertex->anterior== NULL) {
        (*G)->vertices= auxVertex->proximo;
        if ((*G)->vertices!= NULL) (*G)->vertices->anterior= NULL;

        (*ID)= auxIdVertex->proximo;
        if ((*ID)!= NULL) (*ID)->anterior= NULL;
    }
    else {
        if (auxVertex->proximo!= NULL) auxVertex->proximo->anterior= auxVertex->anterior;
        auxVertex->anterior->proximo= auxVertex->proximo;

        if (auxIdVertex->proximo!= NULL) auxIdVertex->proximo->anterior= auxIdVertex->anterior;
        auxIdVertex->anterior->proximo= auxIdVertex->proximo;
    }
    (*G)->numVertices--;

    free(auxVertex);
    free(auxIdVertex);
    return pVertice;
}

void alteraValorVertice(Grafo *G, ListaIdVertex *ID, identificador identificaV, tipoPeso novoValor) {
    ApontadorIdV auxIdVertex= *ID;

    while (auxIdVertex->id!= identificaV) auxIdVertex= auxIdVertex->proximo;

    auxIdVertex->vertice->pVertice= novoValor;
}

tipoPeso valorVertice(ApontadorVertex V, identificador *idV) {
    *idV= V->idV->id;

    return V->pVertice;
}

void verticesFinais(ApontadorArestas A, ApontadorVertex *vA, ApontadorVertex *vB) {
    *vA= A->vertexA;
    *vB= A->vertexB;
}

int saoAdjacentes(ApontadorVertex vA, ApontadorVertex vB) {
    ApontadorArestas arestaA, arestaB;
    ApontadorAux estruturaA= vA->LArestas, estruturaB;

    while(estruturaA!= NULL) {
        arestaA= estruturaA->aresta;
        estruturaB= vB->LArestas;

        while(estruturaB!= NULL) {
            arestaB= estruturaB->aresta;
            if (arestaA== arestaB) return 1;

            estruturaB= estruturaB->proximo;
        }
        estruturaA= estruturaA->proximo;
    }
    return 0;
}

ApontadorVertex verticeOposto(ApontadorVertex vAtual, ApontadorArestas aresta) {
    ApontadorVertex vOposto= aresta->vertexA;

    if (vOposto== vAtual) return vOposto= aresta->vertexB;

    return vOposto;
}

void insereEstruturaAdj(ApontadorVertex vertice, ApontadorArestas aresta, ApontadorAux estrutura) {

    if (vertice->LArestas== NULL) estrutura->proximo= NULL;
    else {
        vertice->LArestas->anterior= estrutura;
        estrutura->proximo= vertice->LArestas;
    }
    estrutura->anterior= NULL;
    estrutura->aresta= aresta;
    vertice->LArestas= estrutura;
}

ApontadorArestas insereAresta(Grafo *G, tipoPeso valorAresta, ListaIdArestas *ID, identificador valorID, ListaIdVertex *IDvert, identificador idV1, identificador idV2) {
    ApontadorArestas auxAresta;
    ApontadorIdA auxIdAresta;
    ApontadorAux estruturaA, estruturaB;
    ApontadorIdV auxIdVertex= *IDvert;
    ApontadorVertex auxVertexA, auxVertexB;

    auxAresta= (ApontadorArestas)malloc(sizeof(TipoNoAresta));
    auxIdAresta= (ApontadorIdA)malloc(sizeof(TipoIdAresta));
    estruturaA= (ApontadorAux)malloc(sizeof(TipoNoAuxiliar));
    estruturaB= (ApontadorAux)malloc(sizeof(TipoNoAuxiliar));

    while(auxIdVertex->id!= idV1 && auxIdVertex->id!= idV2) auxIdVertex= auxIdVertex->proximo;
    auxVertexA= auxIdVertex->vertice;
    if (idV1!= idV2) auxIdVertex= auxIdVertex->proximo;

    while(auxIdVertex->id!= idV1 && auxIdVertex->id!= idV2) auxIdVertex= auxIdVertex->proximo;
    auxVertexB= auxIdVertex->vertice;

    insereEstruturaAdj(auxVertexA, auxAresta, estruturaA);
    insereEstruturaAdj(auxVertexB, auxAresta, estruturaB);

    if ((*G)->arestas!= NULL) (*G)->arestas->anterior= auxAresta;
    auxAresta->pAresta= valorAresta;
    auxAresta->anterior= NULL;
    auxAresta->proximo= (*G)->arestas;
    auxAresta->vertexA= auxVertexA;
    auxAresta->vertexB= auxVertexB;
    auxAresta->adjVertexA= estruturaA;
    auxAresta->adjVertexB= estruturaB;
    auxAresta->idA= auxIdAresta;
    (*G)->numArestas++;

    if ((*ID)!= NULL) (*ID)->anterior= auxIdAresta;
    auxIdAresta->id= valorID;
    auxIdAresta->anterior= NULL;
    auxIdAresta->proximo= (*ID);
    auxIdAresta->aresta= auxAresta;

    (*G)->arestas= auxAresta;
    (*ID)= auxIdAresta;
    return (*G)->arestas;
}

void deletaEstruturaAdj(ApontadorVertex vertice, ApontadorAux estrutura) {

    if ((estrutura->proximo== NULL) && (estrutura->anterior== NULL)) vertice->LArestas= NULL;
    else if (estrutura->proximo== NULL) estrutura->anterior->proximo= NULL;
    else if (estrutura->anterior== NULL) {
        vertice->LArestas= estrutura->proximo;
        estrutura->proximo->anterior= NULL;
    }
    else {
        estrutura->proximo->anterior= estrutura->anterior;
        estrutura->anterior->proximo= estrutura->proximo;
    }
    free(estrutura);
}

tipoPeso deletaAresta(Grafo *G, ListaIdArestas *ID, identificador valorID) {
    ApontadorArestas auxAresta;
    ApontadorIdA auxIdAresta= *ID;
    tipoPeso pAresta;

    while(auxIdAresta->id!= valorID) auxIdAresta= auxIdAresta->proximo;
    auxAresta= auxIdAresta->aresta;
    pAresta= auxAresta->pAresta;

    deletaEstruturaAdj((auxAresta->vertexA), (auxAresta->adjVertexA));
    deletaEstruturaAdj((auxAresta->vertexB), (auxAresta->adjVertexB));

    if (auxAresta->anterior== NULL) {
        (*G)->arestas= auxAresta->proximo;
        if ((*G)->arestas!= NULL) (*G)->arestas->anterior= NULL;

        (*ID)= auxIdAresta->proximo;
        if ((*ID)!= NULL) (*ID)->anterior= NULL;
    }
    else {
        if (auxAresta->proximo!= NULL) auxAresta->proximo->anterior= auxAresta->anterior;
        auxAresta->anterior->proximo= auxAresta->proximo;

        if (auxIdAresta->proximo!= NULL) auxIdAresta->proximo->anterior= auxIdAresta->anterior;
        auxIdAresta->anterior->proximo= auxIdAresta->proximo;
    }
    (*G)->numArestas--;

    free(auxAresta);
    free(auxIdAresta);
    return pAresta;
}

void alteraValorAresta(Grafo *G, ListaIdArestas *ID, identificador identificaA, tipoPeso novoValor) {
    ApontadorIdA auxIdAresta= *ID;

    while (auxIdAresta->id!= identificaA) auxIdAresta= auxIdAresta->proximo;

    auxIdAresta->aresta->pAresta= novoValor;
}

tipoPeso valorAresta(ApontadorArestas A, identificador *idA, identificador *idV1, identificador *idV2) {
    *idA= A->idA->id;
    *idV1= A->vertexA->idV->id;
    *idV2= A->vertexB->idV->id;

    return A->pAresta;
}

void imprimeGrafo(Grafo *G, ListaIdVertex *IDver, ListaIdArestas *IDare) {
    ApontadorIdV auxIdVertex= *IDver;
    ApontadorIdA auxIdArestas= *IDare;
    int i;
    identificador arestaId, vertexIdA, vertexIdB, auxIdentificador;
    tipoPeso pVertice, pAresta;

    if (((*G)->vertices)!= NULL) {
        while(auxIdVertex->proximo!= NULL) auxIdVertex= auxIdVertex->proximo;

        printf("%d", (*G)->numVertices);
        for (i= 0; i< ((*G)->numVertices); i++) {
            pVertice= valorVertice((auxIdVertex->vertice), &vertexIdA);
            printf("\n%d %d", vertexIdA, pVertice);
            auxIdVertex= auxIdVertex->anterior;
        }
    }
    else printf("\n0");

    if (((*G)->arestas)!= NULL) {
        while(auxIdArestas->proximo!= NULL) auxIdArestas= auxIdArestas->proximo;

        printf("\n%d", (*G)->numArestas);
        for (i= 0; i< ((*G)->numArestas); i++) {
            pAresta= valorAresta((auxIdArestas->aresta), &arestaId, &vertexIdA, &vertexIdB);
            if (vertexIdA > vertexIdB) {
                auxIdentificador= vertexIdA;
                vertexIdA= vertexIdB;
                vertexIdB= auxIdentificador;
            }
            printf("\n%d %d %d %d", arestaId, vertexIdA, vertexIdB, pAresta);
            auxIdArestas= auxIdArestas->anterior;
        }
    }
    else printf ("\n0");

    printf("\n");
}

void inicializaFila(TipoFila *F) {
    F->primeiro= NULL;
    F->ultimo= NULL;
}

int filaVazia(TipoFila *F) {
    if ((F->primeiro== NULL) && (F->ultimo== NULL)) return 1;

    return 0;
}

void enfileira(TipoFila *F, tipoItem I) {
    ApontadorFila auxFila;

    auxFila= (ApontadorFila)malloc(sizeof(TipoNoFila));
    auxFila->item= I;

    if (I.vertice->distancia== 0) {
        if (F->primeiro== NULL) F->primeiro= auxFila;
        else (F->ultimo)->proximo= auxFila;
        auxFila->anterior= F->ultimo;
        auxFila->proximo= NULL;
        F->ultimo= auxFila;
    }
    else {
        if (F->ultimo== NULL) F->ultimo= auxFila;
        else (F->primeiro)->anterior= auxFila;
        auxFila->anterior= NULL;
        auxFila->proximo= F->primeiro;
        F->primeiro= auxFila;
    }
}

void desenfileira(TipoFila *F, tipoItem *I) {
    ApontadorFila auxFila= F->ultimo;

    if (F->primeiro== auxFila) inicializaFila(F);
    else {
        auxFila->anterior->proximo= NULL;
        F->ultimo= auxFila->anterior;
    }
    (*I)= auxFila->item;

    free(auxFila);
}

void reorganizaFila(TipoFila *F, ApontadorVertex V) {
    ApontadorFila auxFila= F->primeiro;
    tipoItem auxItem;

    while(auxFila->item.vertice!= V) auxFila= auxFila->proximo;

    while((auxFila->proximo!= NULL) && ((auxFila->item.vertice->distancia)< (auxFila->proximo->item.vertice->distancia))) {
        auxItem= auxFila->item;
        auxFila->item= auxFila->proximo->item;
        auxFila->proximo->item= auxItem;

        auxFila= auxFila->proximo;
    }
}

void caminhoMinimo(Grafo *G, ApontadorVertex auxVertex, identificador idVorigem, identificador idVdestino, int flag) {

    if (flag== 0) {
        auxVertex= dijkstra(G, idVorigem, idVdestino);
        printf("%d\n", (auxVertex->distancia));
    }
    if (auxVertex->idV->id!= idVorigem) caminhoMinimo(G, auxVertex->pai, idVorigem, idVdestino, 1);

    if (auxVertex->idV->id!= idVdestino) printf("%d ", auxVertex->idV->id);
    else printf("%d", auxVertex->idV->id);

    if (flag== 0) printf("\n");
}

ApontadorVertex dijkstra(Grafo *G, identificador idVorigem, identificador idVdestino) {
    TipoFila fila;
    tipoItem auxItem;
    tipoPeso pVertex, pAresta;
    identificador idVertice1, idVertice2, idAresta;
    ApontadorVertex auxVertex1= (*G)->vertices, auxVertex2, auxVertex3;
    ApontadorAux auxAdjacente;

    inicializaFila(&fila);

    do {
        pVertex= valorVertice(auxVertex1, &idVertice1);
        auxVertex1->pai= NULL;
        if (idVertice1== idVdestino) auxVertex3= auxVertex1;

        if (idVertice1== idVorigem) auxVertex1->distancia= 0;
        else auxVertex1->distancia= MAXDIST;

        auxItem.vertice= auxVertex1;
        enfileira(&fila, auxItem);
        auxVertex1= auxVertex1->proximo;
    } while(auxVertex1!= NULL);

    while(!filaVazia(&fila)) {
        desenfileira(&fila, &auxItem);
        auxVertex1= auxItem.vertice;
        auxAdjacente= auxVertex1->LArestas;

        if (auxAdjacente!= NULL) {
            do {
                auxVertex2= verticeOposto(auxVertex1, (auxAdjacente->aresta));
                pAresta= valorAresta((auxAdjacente->aresta), &idAresta, &idVertice1, &idVertice2);

                if ((auxVertex2->distancia)> (auxVertex1->distancia + pAresta)) {
                    auxVertex2->distancia= auxVertex1->distancia + pAresta;
                    auxVertex2->pai= auxVertex1;

                    reorganizaFila(&fila, auxVertex2);
                }
                auxAdjacente= auxAdjacente->proximo;
            } while(auxAdjacente!= NULL);
        }
    }
    return auxVertex3;
}

void finalizaGrafo(Grafo *G, ListaIdVertex *IDvert, ListaIdArestas *IDare) {
    ApontadorIdV auxIdVertex;

    while((*G)->vertices!= NULL) {
        auxIdVertex= *IDvert;
        deletaVertice(G, IDvert, (auxIdVertex->id), IDare);
    }
    free(*G);
}
