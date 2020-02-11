/*  Algoritmos e Estruturas de Dados II - Trabalho 2 - Árvores-B
          Evandro Scudeleti Ortigossa, n° USP 6793135
          Bacharelado em Ciências de Computação           */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define REGTAM 256
#define PAGTAM 4
#define CARQTDE sizeof(long)
#define PAGBYTE (CARQTDE*(3*PAGTAM + 1))    /* Definição das principais constantes utilizadas no sistema */
#define DELIMITADOR '@'
#define DELIMITACAO "%[^@]s"
#define PREENCHE '#'
#define EXCLUIDO "*|"

typedef char tipoCodigo[6];
typedef char *tipoString;

typedef struct {                            /* Estrutura que recebe os dados de uma referência */
    tipoCodigo codigo;
    tipoString titulo;
    tipoString autor;
    tipoString anoPublicacao;
    tipoString veiculo;
} Referencia;

typedef struct {                            /* Estrutura que recebe uma página completa de Árvore-B */
    long pagEndereco;
    long offsetPagina[PAGTAM+1];
    int codigoReg[PAGTAM];
    long offsetReg[PAGTAM];
} ArvoreB;

void criaArquivos(FILE *, FILE *);
void processaComando(int *);
void recebeReferencia(FILE *, FILE *, long);
void imprimeReferencia(FILE *, FILE *, Referencia, long);
void imprimeIndice(FILE *, ArvoreB);
void inicializaIndice(FILE *, int);
void inicializaPagina(ArvoreB *, long);
int paginaCheia(ArvoreB);
void insereOrdenado(FILE *, ArvoreB *, int, long, long);
void splitDeRaiz(FILE *, ArvoreB *, int, long, long);
long split(FILE *, ArvoreB *, int *, long *, long);
long pop(FILE *);
void insereIndice(FILE *, int, long, long, int);
void alteraReferencia(FILE *, FILE *);
void removeReferencia(FILE *, FILE *);
int verificaFolha(ArvoreB);
void push(FILE *, ArvoreB);
int removeOrdenado(ArvoreB *, int);
int redistribuicao(FILE *, ArvoreB *, int, int *);
int concatenacao(FILE *, ArvoreB *, int *, int, long);
void buscaMenor(FILE *, ArvoreB *, int *, int *);
void removeIndice(FILE *, ArvoreB, int, int);
void reorganizaArvore(FILE *, ArvoreB *, int, int, long);
long buscaReferecia(FILE *, FILE *);
long pesquisaArquivo(FILE *, ArvoreB *, int, int *);
void finalizaExecucao(FILE *, FILE *);

int main(void) {
    int comando;
    FILE *indice, *dados;

    do {
        indice= fopen("index.dat", "r+b");
        dados= fopen("data.txt", "r+");
        if ((indice== NULL) || (dados== NULL)) criaArquivos(indice, dados);
    } while((indice== NULL) || (dados== NULL));

    do {
        printf("Comandos:\n- Inserir\n- Alterar\n- Remover\n- Buscar\n- Finalizar\n\n");
        processaComando(&comando);

        if (comando== 1) recebeReferencia(indice, dados, -1);
        else if (comando== 2) alteraReferencia(indice, dados);
        else if (comando== 3) removeReferencia(indice, dados);
        else if (comando== 4) buscaReferecia(indice, dados);

        printf("\n");
    } while(comando< 5);

    finalizaExecucao(indice, dados);

return 0;

}

void criaArquivos(FILE *indice, FILE *dados) {

    if (indice== NULL) {
        indice= fopen("index.dat", "wb");        /* Se não existirem, esta função cria os arquivos de índice e dados */
        fclose(indice);
        indice= NULL;
    }
    if (dados== NULL) {
        dados= fopen("data.txt", "w");
        fclose(dados);
        dados= NULL;
    }
}

void processaComando(int *opcao) {
    char comando[20]= " ";

    scanf("\n");
    gets(comando);

    if (strcasecmp(comando, "INSERIR")== 0) (*opcao)= 1;
    else if (strcasecmp(comando, "ALTERAR")== 0) (*opcao)= 2;
    else if (strcasecmp(comando, "REMOVER")== 0) (*opcao)= 3;        /* Processamento de comandos inseridos pelo usuário */
    else if (strcasecmp(comando, "BUSCAR")== 0) (*opcao)= 4;
    else if (strcasecmp(comando, "FINALIZAR")== 0) (*opcao)= 5;
    else (*opcao)= 0;

    if ((*opcao)> 0 && (*opcao)< 5) {
        system("clear");
        printf("%s\n\n", comando);
    }
}

void recebeReferencia(FILE *indice, FILE *dados, long posicao) {
    char auxChar[REGTAM]= " ";
    Referencia registro;

    if (posicao== -1) {
        printf("Codigo: ");
        scanf("\n");
        gets(registro.codigo);              /* Respectivos dados de uma referência são captados */
    }
    else {
        fseek(dados, posicao, SEEK_SET);
        fgets(auxChar, REGTAM, dados);
        sscanf(auxChar, DELIMITACAO, registro.codigo);
        printf("\n");
    }
    printf("Titulo: ");
    scanf("\n");
    gets(auxChar);
    registro.titulo= (tipoString)malloc(sizeof(tipoString)*(strlen(auxChar)));
    strcpy(registro.titulo, auxChar);
    printf("Autor: ");
    scanf("\n");
    gets(auxChar);
    registro.autor= (tipoString)malloc(sizeof(tipoString)*(strlen(auxChar)));
    strcpy(registro.autor, auxChar);
    printf("Ano de Publicacao: ");
    scanf("\n");
    gets(auxChar);
    registro.anoPublicacao= (tipoString)malloc(sizeof(tipoString)*(strlen(auxChar)));
    strcpy(registro.anoPublicacao, auxChar);
    printf("Veiculo: ");
    scanf("\n");
    gets(auxChar);
    registro.veiculo= (tipoString)malloc(sizeof(tipoString)*(strlen(auxChar)));
    strcpy(registro.veiculo, auxChar);
    printf("\n");

    imprimeReferencia(indice, dados, registro, posicao);

    free(registro.titulo);
    free(registro.autor);
    free(registro.anoPublicacao);                /* Liberação de memória alocada pelo registro de referências */
    free(registro.veiculo);
}

void imprimeReferencia(FILE *indice, FILE *dados, Referencia registro, long posicao) {
    int tamanho, codigo= 0, i;
    long byteOffset= 0;

    if (posicao== -1) {
        fseek(dados, 0, SEEK_END);
        byteOffset= ftell(dados);
    }
    else fseek(dados, posicao, SEEK_SET);
    tamanho= (strlen(registro.codigo) + strlen(registro.titulo) + strlen(registro.autor) + strlen(registro.anoPublicacao) + strlen(registro.veiculo));

    for ((i= 0); (i<= REGTAM); (i++)) {
        if ((i>= 0) && (i<= 4)) {
            if (i== 0) fprintf(dados, "%s", registro.codigo);
            else if (i== 1) fprintf(dados, "%s", registro.titulo);
            else if (i== 2) fprintf(dados, "%s", registro.autor);
            else if (i== 3) fprintf(dados, "%s", registro.anoPublicacao);
            else if (i== 4) fprintf(dados, "%s", registro.veiculo);

            fputc(DELIMITADOR, dados);           /* Impressão dos dados da referência, delimitadores de campo e */
            tamanho++;                           /* caracteres de preenchimento no final do arquivo de dados */
        }
        else if (i== 5) i= tamanho;
        else fputc(PREENCHE, dados);
    }
    if (posicao== -1) {
        for ((i= 0); (i< strlen(registro.codigo)); (i++)) codigo += registro.codigo[i];
        insereIndice(indice, codigo, byteOffset, -1, codigo);
    }
}

void imprimeIndice(FILE *indice, ArvoreB pagina) {
    int i;

    if (pagina.pagEndereco== -1) fseek(indice, 0, SEEK_END);
    else fseek(indice, pagina.pagEndereco, SEEK_SET);

    for ((i= 0); (i< PAGTAM); (i++)) {
        fwrite(&(pagina.offsetPagina[i]), CARQTDE, 1, indice);       /* Impressão de uma página completa da Árvore-B */
        fwrite(&(pagina.codigoReg[i]), CARQTDE, 1, indice);
        fwrite(&(pagina.offsetReg[i]), CARQTDE, 1, indice);
    }
    fwrite(&(pagina.offsetPagina[i]), CARQTDE, 1, indice);
}

void inicializaIndice(FILE *indice, int codigo) {
    int topoPilha= -1;
    long raiz= 2*CARQTDE;
    ArvoreB pagina;

    fwrite(&raiz, CARQTDE, 1, indice);           /* Inicialização do arquivo de índices com os dados do primeiro */
    fwrite(&topoPilha, CARQTDE, 1, indice);      /* registro inserido no arquivo de dados */
    inicializaPagina(&pagina, -1);
    pagina.codigoReg[PAGTAM-1]= codigo;
    imprimeIndice(indice, pagina);
}

void inicializaPagina(ArvoreB *pagina, long endereco) {
    int i;

    for ((i= 0); (i< PAGTAM); (i++)) {
        (*pagina).offsetPagina[i]= -1;           /* Inicialização de uma página de Árvore-B */
        (*pagina).codigoReg[i]= 0;
        (*pagina).offsetReg[i]= 0;
    }
    (*pagina).offsetPagina[i]= -1;
    (*pagina).pagEndereco= endereco;
}

int paginaCheia(ArvoreB pagina) {

    if (pagina.codigoReg[0]== 0) return 0;
    else return 1;
}

void insereOrdenado(FILE *indice, ArvoreB *pagina, int codigo, long posRegistro, long ponteiro) {
    int posInsercao= -1, i;

    for ((i= (PAGTAM-1)); ((i>= 0) && (posInsercao== -1)); (i--)) if ((*pagina).codigoReg[i]< codigo) posInsercao= i;
    for ((i= 0); (i< posInsercao); (i++)) {
        (*pagina).offsetPagina[i]= (*pagina).offsetPagina[i+1];      /* Posição ideal para inserção de novo dado é encontrada */
        (*pagina).codigoReg[i]= (*pagina).codigoReg[i+1];            /* Demais dados são deslocados para receber o novo dado */
        (*pagina).offsetReg[i]= (*pagina).offsetReg[i+1];
        (*pagina).offsetPagina[i+1]= (*pagina).offsetPagina[i+2];
    }
    if ((*pagina).codigoReg[posInsercao]== 0) (*pagina).offsetPagina[posInsercao]= (*pagina).offsetPagina[posInsercao+1];
    (*pagina).codigoReg[posInsercao]= codigo;
    (*pagina).offsetReg[posInsercao]= posRegistro;
    (*pagina).offsetPagina[posInsercao+1]= ponteiro;
    imprimeIndice(indice, (*pagina));
}

void splitDeRaiz(FILE *indice, ArvoreB *pagina, int codigo, long posRegistro, long ponteiro) {
    long topoPagDel, posicaoPagina;
    ArvoreB splitRaiz;

    posicaoPagina= split(indice, pagina, &codigo, &posRegistro, ponteiro);
    topoPagDel= pop(indice);

    if (topoPagDel== -1) inicializaPagina(&splitRaiz, ftell(indice));
    else inicializaPagina(&splitRaiz, topoPagDel);

    splitRaiz.offsetPagina[PAGTAM-1]= (*pagina).pagEndereco;    /* Criação e impressão de nova raiz */
    splitRaiz.codigoReg[PAGTAM-1]= codigo;
    splitRaiz.offsetReg[PAGTAM-1]= posRegistro;
    splitRaiz.offsetPagina[PAGTAM]= posicaoPagina;

    imprimeIndice(indice, splitRaiz);

    fseek(indice, 0, SEEK_SET);
    fwrite(&(splitRaiz.pagEndereco), CARQTDE, 1, indice);
}

long split(FILE *indice, ArvoreB *pagina, int *codigo, long *posRegistro, long ponteiro) {
    int auxCodigo= *codigo, i;
    long topoPagDel, auxPosReg= *posRegistro, offSet1, offSet2= ponteiro;
    ArvoreB splitPagina, auxPagina;

    topoPagDel= pop(indice);
    if (topoPagDel== -1) inicializaPagina(&splitPagina, ftell(indice));   /* Nova página é inserida na posição indicada pela pilha de excluídos */
    else inicializaPagina(&splitPagina, topoPagDel);

    inicializaPagina(&auxPagina, (*pagina).pagEndereco);

    for ((i= (PAGTAM/2)); (i< PAGTAM); (i++)) {       /* Maiores valores de dados são passados para nova página */
        splitPagina.codigoReg[i]= (*pagina).codigoReg[i];
        splitPagina.offsetReg[i]= (*pagina).offsetReg[i];
        splitPagina.offsetPagina[i+1]= (*pagina).offsetPagina[i+1];
    }
    for ((i= 0); (i< (PAGTAM/2)); (i++)) {            /* Demais valores da página são passados para um auxiliar de forma ordenada*/
        auxPagina.offsetPagina[PAGTAM-1-i]= (*pagina).offsetPagina[(PAGTAM/2)-1-i];
        auxPagina.codigoReg[PAGTAM-1-i]= (*pagina).codigoReg[(PAGTAM/2)-1-i];
        auxPagina.offsetReg[PAGTAM-1-i]= (*pagina).offsetReg[(PAGTAM/2)-1-i];
        auxPagina.offsetPagina[PAGTAM-i]= (*pagina).offsetPagina[(PAGTAM/2)-i];
    }
    if (auxCodigo< auxPagina.codigoReg[PAGTAM-1]) {
        for ((i= (PAGTAM/2)); (i< PAGTAM); (i++)) {
            if (auxCodigo< auxPagina.codigoReg[i]) {
                (*codigo)= auxPagina.codigoReg[i];
                (*posRegistro)= auxPagina.offsetReg[i];
                offSet1= auxPagina.offsetPagina[i+1];

                auxPagina.codigoReg[i]= auxCodigo;
                auxPagina.offsetReg[i]= auxPosReg;
                auxPagina.offsetPagina[i+1]= offSet2;

                auxCodigo= *codigo;
                auxPosReg= *posRegistro;
                offSet2= offSet1;
            }
        }
        splitPagina.offsetPagina[PAGTAM/2]= offSet2;
    }              /* Busca da posição ideal para o novo dado e retorno do elemento intermediário para a página "raiz" */
    else {
        for ((i= (PAGTAM-1)); (i>= (PAGTAM/2)); (i--)) {
            if (auxCodigo> splitPagina.codigoReg[i]) {
                (*codigo)= splitPagina.codigoReg[i];
                (*posRegistro)= splitPagina.offsetReg[i];
                offSet1= splitPagina.offsetPagina[i+1];

                splitPagina.codigoReg[i]= auxCodigo;
                splitPagina.offsetReg[i]= auxPosReg;
                splitPagina.offsetPagina[i+1]= offSet2;

                auxCodigo= *codigo;
                auxPosReg= *posRegistro;
                offSet2= offSet1;
            }
        }
        splitPagina.offsetPagina[i+1]= offSet2;
    }
    imprimeIndice(indice, auxPagina);
    imprimeIndice(indice, splitPagina);

    return splitPagina.pagEndereco;         /* Retorno da posição, no arquivo de índices, da página criada pelo split */
}

long pop(FILE *indice) {
    long posInsercao, topoPilha;

    fseek(indice, CARQTDE, SEEK_SET);
    fread(&posInsercao, CARQTDE, 1, indice);          /* Posição da última página excluída é armazenada para retorno */

    if (posInsercao!= -1) {
        fseek(indice, (posInsercao+(sizeof(EXCLUIDO)-1)), SEEK_SET);
        fread(&topoPilha, CARQTDE, 1, indice);        /* Topo da pilha é atualizado com a informação contida após os */
        fseek(indice, CARQTDE, SEEK_SET);             /* caracteres de exclusão na página indicada pelo topo da pilha */
        fwrite(&topoPilha, CARQTDE, 1, indice);
    }
    fseek(indice, 0, SEEK_END);
    return posInsercao;
}

void insereIndice(FILE *indice, int codigo, long posRegistro, long ponteiro, int auxCodigo) {
    int codigoPai= -1;
    long posicao;
    ArvoreB pagina;

    fseek(indice, 0, SEEK_END);

    if (ftell(indice)== 0) inicializaIndice(indice, codigo);    /* Caso de inserção dos primeiros dados no arquivo de índice */
    else {
        pesquisaArquivo(indice, &pagina, auxCodigo, &codigoPai);

        if (!paginaCheia(pagina)) insereOrdenado(indice, &pagina, codigo, posRegistro, ponteiro);
        else {                                        /* Caso em que há espaço na página que o dado deve ser inserido */
            fseek(indice, 0, SEEK_SET);
            fread(&posicao, CARQTDE, 1, indice);      /* Senão há a necessidade de split de página ou de raiz */

            if (pagina.pagEndereco== posicao) splitDeRaiz(indice, &pagina, codigo, posRegistro, ponteiro);
            else {
                posicao= split(indice, &pagina, &codigo, &posRegistro, ponteiro);
                insereIndice(indice, codigo, posRegistro, posicao, codigoPai);
            }
        }
    }
}

void alteraReferencia(FILE *indice, FILE *dados) {
    long byteOffset;

    byteOffset= buscaReferecia(indice, dados);        /* Se a referência existir, será alterada pelo usuário */

    if (byteOffset!= -1) recebeReferencia(indice, dados, byteOffset);
}

void removeReferencia(FILE *indice, FILE *dados) {
    int codigo= 0, codigoPai= -1, i;
    long byteOffset;
    char codChar[6]= " ";
    ArvoreB pagina;

    printf("Insira o codigo da referencia: ");
    scanf("\n");
    gets(codChar);
    for ((i= 0); (i< strlen(codChar)); (i++)) codigo += codChar[i];
    byteOffset= pesquisaArquivo(indice, &pagina, codigo, &codigoPai);     /* Pesquisa pelo código da referência no arquivo de índices */

    if (byteOffset< 0) printf("\n\nReferencia inexistente!!!\n\n");       /* Retorno de seu respectivo byteoffset, se -1, arquivo não existe */
    else {
        fseek(dados, byteOffset, SEEK_SET);
        fprintf(dados, "%s", EXCLUIDO);
        removeIndice(indice, pagina, codigo, codigoPai);                  /* Exclusão no arquivo de dados e do respectivo índice */
        printf("\n\nReferencia removida com sucesso!!!\n\n");
    }
}

int verificaFolha(ArvoreB pagina) {

    if (pagina.offsetPagina[PAGTAM]== -1) return 1;
    else return 0;      /* Páginas são mantidas em ordem, logo se pode verificar a existência de filhos na última posição do vetor */
}

void push(FILE *indice, ArvoreB pagina) {
    long proxTopo;

    fseek(indice, CARQTDE, SEEK_SET);
    fread(&proxTopo, CARQTDE, 1, indice);        /* Valor atual do topo da pilha é armazenado */
    fseek(indice, CARQTDE, SEEK_SET);
    fwrite(&(pagina.pagEndereco), CARQTDE, 1, indice);     /* Endereço do novo topo da pilha é escrito no lugar do antigo */

    fseek(indice, pagina.pagEndereco, SEEK_SET);
    fwrite(EXCLUIDO, (sizeof(EXCLUIDO)-1), 1, indice);     /* Página recém excluída recebe os caracteres de exclusão e o antigo topo da pilha */
    fwrite(&(proxTopo), CARQTDE, 1, indice);
}

int removeOrdenado(ArvoreB *pagina, int codigo) {
    int posRemocao= -1, posVazias= 1, i;

    for ((i= 0); ((i< PAGTAM) && (posRemocao== -1)); (i++)) {
        if ((*pagina).codigoReg[i]== codigo) posRemocao= i;
        else if ((*pagina).codigoReg[i]== 0) posVazias++;
    }
    for ((i= posRemocao); (i>= 0); (i--)) {
        if ((i> 0) && ((*pagina).codigoReg[i]!= 0)) {
            (*pagina).codigoReg[i]= (*pagina).codigoReg[i-1];
            (*pagina).offsetReg[i]= (*pagina).offsetReg[i-1];
            (*pagina).offsetPagina[i+1]= (*pagina).offsetPagina[i];
        }
        else if ((i== 0) && ((*pagina).codigoReg[i+1]== 0)) (*pagina).offsetPagina[i+1]= -1;
    }
    (*pagina).offsetPagina[i+1]= -1;             /* Função responsável por fazer a exclusão de um elemento da página e retornar */
    (*pagina).codigoReg[i+1]= 0;                 /* sua situação de Underflow, caso exista, retorna verdade */
    (*pagina).offsetReg[i+1]= 0;

    if (posVazias> (PAGTAM/2)) return 1;
    else return 0;
}

int redistribuicao(FILE *indice, ArvoreB *pagina, int codigoPai, int *posOffSet) {
    int posVizVazias, posPagVazias, posRemocao, auxCodigo= -1, sucesso= 0, i, j;
    ArvoreB paginaPai, paginaDireita, paginaEsquerda;

    pesquisaArquivo(indice, &paginaPai, codigoPai, &auxCodigo);

    for ((i= 0); (i<= PAGTAM); (i++)) {
        if (paginaPai.offsetPagina[i]== (*pagina).pagEndereco) {
            (*posOffSet)= i;
            posPagVazias= PAGTAM-(PAGTAM/2);     /* Neste caso, se possível, os elementos da página com underflow são */
                                                 /* redistribuídos com os da página irmã direita */
            if (i< PAGTAM) {
                fseek(indice, (paginaPai.offsetPagina[i+1]), SEEK_SET);
                paginaDireita.pagEndereco= paginaPai.offsetPagina[i+1];
                posVizVazias= 0, posRemocao= -1;

                for ((j= 0); (j< PAGTAM); (j++)) {
                    fread(&(paginaDireita.offsetPagina[j]), CARQTDE, 1, indice);
                    fread(&(paginaDireita.codigoReg[j]), CARQTDE, 1, indice);
                    fread(&(paginaDireita.offsetReg[j]), CARQTDE, 1, indice);

                    if (paginaDireita.codigoReg[j]== 0) posVizVazias++;
                    if ((paginaDireita.codigoReg[j]!= 0) && (posRemocao== -1)) posRemocao= j;
                }
                fread(&(paginaDireita.offsetPagina[j]), CARQTDE, 1, indice);

                if ((posVizVazias< (PAGTAM/2)) && (verificaFolha(paginaDireita))) {
                    do {
                        insereOrdenado(indice, pagina, (paginaPai.codigoReg[i]), (paginaPai.offsetReg[i]), (paginaDireita.offsetPagina[posRemocao]));
                        paginaPai.codigoReg[i]= paginaDireita.codigoReg[posRemocao];
                        paginaPai.offsetReg[i]= paginaDireita.offsetReg[posRemocao];
                        removeOrdenado(&paginaDireita, (paginaPai.codigoReg[i]));

                        posPagVazias--;          /* Se a página irmã direita apresentar elementos suficientes, */
                        posVizVazias++;          /* processo de redistribuição é iniciado */
                        posRemocao++;
                    } while(posPagVazias> posVizVazias);

                    imprimeIndice(indice, paginaPai);
                    imprimeIndice(indice, paginaDireita);
                    return (sucesso= 1);         /* Retorno de informação de operação bem sucedida */
                }
            }
            if ((!sucesso) && (i> 0) && (paginaPai.offsetPagina[i-1]!= -1)) {
                fseek(indice, (paginaPai.offsetPagina[i-1]), SEEK_SET);
                paginaEsquerda.pagEndereco= paginaPai.offsetPagina[i-1];
                posVizVazias= 0;       /* Neste caso, se não houve redistribuição com a página irmã direita, se possível, */
                                       /* os elementos da página com underflow são redistribuídos com os da página irmã esquerda */
                for ((j= 0); (j< PAGTAM); (j++)) {
                    fread(&(paginaEsquerda.offsetPagina[j]), CARQTDE, 1, indice);
                    fread(&(paginaEsquerda.codigoReg[j]), CARQTDE, 1, indice);
                    fread(&(paginaEsquerda.offsetReg[j]), CARQTDE, 1, indice);

                    if (paginaEsquerda.codigoReg[j]== 0) posVizVazias++;
                }
                fread(&(paginaEsquerda.offsetPagina[j]), CARQTDE, 1, indice);

                if ((posVizVazias< (PAGTAM/2)) && (verificaFolha(paginaEsquerda))) {
                    do {
                        (*pagina).codigoReg[posPagVazias]= paginaPai.codigoReg[i-1];
                        (*pagina).offsetReg[posPagVazias]= paginaPai.offsetReg[i-1];
                        (*pagina).offsetPagina[posPagVazias]= paginaEsquerda.offsetPagina[PAGTAM];

                        paginaPai.codigoReg[i-1]= paginaEsquerda.codigoReg[PAGTAM-1];
                        paginaPai.offsetReg[i-1]= paginaEsquerda.offsetReg[PAGTAM-1];

                        removeOrdenado(&paginaEsquerda, (paginaPai.codigoReg[i-1]));
                        posPagVazias--;          /* Se a página irmã esquerda apresentar elementos suficientes, */
                        posVizVazias++;          /* processo de redistribuição é iniciado */
                    } while(posPagVazias> posVizVazias);

                    imprimeIndice(indice, (*pagina));
                    imprimeIndice(indice, paginaPai);
                    imprimeIndice(indice, paginaEsquerda);
                    return (sucesso= 1);         /* Operação bem sucedida */
                }
            }
            i= PAGTAM + 1;
        }
    }
    return sucesso;          /* Retorno de informação de operação bem sucedida ou não */
}

int concatenacao(FILE *indice, ArvoreB *pagina, int *codigoPai, int posOffSet, long raiz) {
    int posVizVazias, posPagVazias, auxCodigo= -1, sucesso= 0, underFlow= 0, i;
    ArvoreB paginaPai, auxPagina;

    pesquisaArquivo(indice, &paginaPai, (*codigoPai), &auxCodigo);
    (*codigoPai)= auxCodigo;
    posPagVazias= PAGTAM-(PAGTAM/2);

    if (posOffSet< PAGTAM) {
        fseek(indice, paginaPai.offsetPagina[posOffSet+1], SEEK_SET);
        auxPagina.pagEndereco= paginaPai.offsetPagina[posOffSet+1];
        posVizVazias= 0;                         /* Neste caso os elementos da página em underflow são concatenados */
                                                 /* com os da página irmã direita */
        for ((i= 0); (i< PAGTAM); (i++)) {
            fread(&(auxPagina.offsetPagina[i]), CARQTDE, 1, indice);
            fread(&(auxPagina.codigoReg[i]), CARQTDE, 1, indice);
            fread(&(auxPagina.offsetReg[i]), CARQTDE, 1, indice);

            if (auxPagina.codigoReg[i]== 0) posVizVazias++;
        }
        fread(&(auxPagina.offsetPagina[i]), CARQTDE, 1, indice);

        if (posVizVazias== (PAGTAM/2)) {         /* Processo de concatenação é iniciado */
            auxPagina.codigoReg[posVizVazias-1]= paginaPai.codigoReg[posOffSet];
            auxPagina.offsetReg[posVizVazias-1]= paginaPai.offsetReg[posOffSet];

            underFlow= removeOrdenado(&paginaPai, paginaPai.codigoReg[posOffSet]);

            if ((raiz== paginaPai.pagEndereco) && (paginaPai.codigoReg[PAGTAM-1]!= 0)) {
                underFlow= 0;
                paginaPai.offsetPagina[posOffSet+1]= auxPagina.pagEndereco;
                imprimeIndice(indice, paginaPai);
            }
            else if (paginaPai.codigoReg[PAGTAM-1]== 0) {
                push(indice, paginaPai);
                fseek(indice, 0, SEEK_SET);      /* Caso em que a altura da árvore é reduzida: exclusão e empilhamento da antiga raiz */
                fwrite(&(auxPagina.pagEndereco), CARQTDE, 1, indice);
            }
            else {
                paginaPai.offsetPagina[posOffSet+1]= auxPagina.pagEndereco;
                imprimeIndice(indice, paginaPai);
            }
            posVizVazias--;
            auxPagina.offsetPagina[posVizVazias-1]= (*pagina).offsetPagina[PAGTAM-1];
            auxPagina.codigoReg[posVizVazias-1]= (*pagina).codigoReg[PAGTAM-1];
            auxPagina.offsetReg[posVizVazias-1]= (*pagina).offsetReg[PAGTAM-1];
            auxPagina.offsetPagina[posVizVazias]= (*pagina).offsetPagina[PAGTAM];
            sucesso= 1;                /* Operação bem sucedida */
        }
    }
    if ((!sucesso) && (posOffSet> 0) && (paginaPai.offsetPagina[posOffSet-1]!= -1)) {
        fseek(indice, paginaPai.offsetPagina[posOffSet-1], SEEK_SET);
        auxPagina.pagEndereco= paginaPai.offsetPagina[posOffSet-1];
        posVizVazias= 0;               /* Neste caso, se a concatenação com a página irmã direita não foi bem sucedida, os */
                                       /* elementos da página em underflow são concatenados com os da página irmã esquerda */
        for ((i= 0); (i< PAGTAM); (i++)) {
            fread(&(auxPagina.offsetPagina[i]), CARQTDE, 1, indice);
            fread(&(auxPagina.codigoReg[i]), CARQTDE, 1, indice);
            fread(&(auxPagina.offsetReg[i]), CARQTDE, 1, indice);

            if (auxPagina.codigoReg[i]== 0) posVizVazias++;
        }
        fread(&(auxPagina.offsetPagina[i]), CARQTDE, 1, indice);

        if (posVizVazias== (PAGTAM/2)) {         /* Processo de concatenação é iniciado */
            insereOrdenado(indice, &auxPagina, paginaPai.codigoReg[posOffSet-1], paginaPai.offsetReg[posOffSet-1], (*pagina).offsetPagina[PAGTAM-1]);
            insereOrdenado(indice, &auxPagina, (*pagina).codigoReg[PAGTAM-1], (*pagina).offsetReg[PAGTAM-1], (*pagina).offsetPagina[PAGTAM]);

            underFlow= removeOrdenado(&paginaPai, paginaPai.codigoReg[posOffSet-1]);

            if ((raiz== paginaPai.pagEndereco) && (paginaPai.codigoReg[PAGTAM-1]!= 0)) {
                underFlow= 0;
                paginaPai.offsetPagina[posOffSet]= auxPagina.pagEndereco;
                imprimeIndice(indice, paginaPai);
            }
            else if (paginaPai.codigoReg[PAGTAM-1]== 0) {
                push(indice, paginaPai);
                fseek(indice, 0, SEEK_SET);      /* Altura da árvore é reduzida: exclusão e empilhamento da antiga raiz */
                fwrite(&(auxPagina.pagEndereco), CARQTDE, 1, indice);
            }
            else {
                paginaPai.offsetPagina[posOffSet]= auxPagina.pagEndereco;
                imprimeIndice(indice, paginaPai);
            }
        }
    }
    push(indice, (*pagina));           /* Página concatenada é excluída e empilhada */
    imprimeIndice(indice, auxPagina);
    (*pagina)= paginaPai;    /* Note que quem retorna é a página "pai" da página concatenada: tratamento de underflow propagado */

    return underFlow;        /* Retorno de informação sobre propagação de underflow */
}

void buscaMenor(FILE *indice, ArvoreB *pagina, int *codigo, int *codigoPai) {
    int posTroca= 0, flag= 0, i;
    long posPagina= 0, auxOffsetReg= 0;
    ArvoreB auxPagina;

    for ((i= (PAGTAM-1)); (i>= 0); (i--)) if ((*pagina).codigoReg[i]== (*codigo)) {
        posTroca= i;
        posPagina= (*pagina).offsetPagina[i+1];  /* Busca pela posição, na página interna, do elemento a ser excluído */
        i= -1;
    }
    do {
        fseek(indice, posPagina, SEEK_SET);      /* Busca pela folha que contém o elemento adequado para troca com o elemento */
        auxPagina.pagEndereco= posPagina;        /* a ser excluído na página interna: menor elemento da página mais a esquerda */

        if (!flag) {
            for ((i= 0); (i< PAGTAM); (i++)) if (auxPagina.codigoReg[i]!= 0) {
                (*codigoPai)= auxPagina.codigoReg[i];
                i= PAGTAM;
            }
        }
        flag= 1;

        for ((i= 0); (i< PAGTAM); (i++)) {
            fread(&(auxPagina.offsetPagina[i]), CARQTDE, 1, indice);
            fread(&(auxPagina.codigoReg[i]), CARQTDE, 1, indice);
            fread(&(auxPagina.offsetReg[i]), CARQTDE, 1, indice);    /* Processamento das páginas até encontrar a adequada */

            if ((flag) && (auxPagina.codigoReg[i]!= 0)) {
                (*codigo)= auxPagina.codigoReg[i];
                auxOffsetReg= auxPagina.offsetReg[i];      /* Armazenamento do menor elemento da página encontrada */
                posPagina= auxPagina.offsetPagina[i];
                flag= 0;
            }
        }
        fread(&(auxPagina.offsetPagina[i]), CARQTDE, 1, indice);
    } while(!verificaFolha(auxPagina));

    (*pagina).codigoReg[posTroca]= (*codigo);    /* Troca com o menor elemento da página mais a esquerda: "exclusão" */
    (*pagina).offsetReg[posTroca]= auxOffsetReg;
    imprimeIndice(indice, (*pagina));

    (*pagina)= auxPagina;              /* Note que quem volta para a função remoção é a página folha e o elemento usado */
}                                      /* como troca, seguindo a regra de só realizar verdadeiras exclusões em folhas */

void removeIndice(FILE *indice, ArvoreB pagina, int codigo, int codigoPai) {
    int underFlow= 0;
    long raiz;

    fseek(indice, 0, SEEK_SET);
    fread(&raiz, CARQTDE, 1, indice);
                                       /* Exclusão de elemento em página interna, busca de um substituto em folha */
    if (!verificaFolha(pagina)) buscaMenor(indice, &pagina, &codigo, &codigoPai);
                                       /* Note que a página anteriormente interna volta como folha */
    underFlow= removeOrdenado(&pagina, codigo);
                                       /* Se houver underflow, necessidade de reorganização */
    if ((underFlow) && (pagina.pagEndereco!= raiz)) reorganizaArvore(indice, &pagina, codigoPai, underFlow, raiz);
    else imprimeIndice(indice, pagina);
}

void reorganizaArvore(FILE *indice, ArvoreB *pagina, int codigoPai, int underFlow, long raiz) {
    int redistribuiu= 1, posOffSet= 0;

    redistribuiu= redistribuicao(indice, pagina, codigoPai, &posOffSet);
                                       /* Redistribuição pode ou não ser aplicada, se não: concatenação */
    if (!redistribuiu) {
        underFlow= concatenacao(indice, pagina, &codigoPai, posOffSet, raiz);
        if (underFlow) reorganizaArvore(indice, pagina, codigoPai, underFlow, raiz);
    }                                  /* Se o underflow se propagou, volto recursivamente reorganizando as páginas "pai" */
}

long buscaReferecia(FILE *indice, FILE *dados) {
    int codigo= 0, posicao= 0, codigoPai= -1, i;
    long byteOffset;
    char codChar[6]= " ", registro[REGTAM]= " ", campo[REGTAM]= " ";
    ArvoreB pagina;

    printf("Insira o codigo da referencia: ");
    scanf("\n");
    gets(codChar);
    for ((i= 0); (i< strlen(codChar)); (i++)) codigo += codChar[i];
    byteOffset= pesquisaArquivo(indice, &pagina, codigo, &codigoPai);

    if (byteOffset< 0) printf("\n\nReferencia inexistente!!!\n\n");
    else {
        fseek(dados, byteOffset, SEEK_SET);      /* Após pesquisa bem sucedida, os campos do registro são separados pelos */
        fgets(registro, REGTAM, dados);          /* delimitadores de campo e impressos para o usuário */

        for ((i= 0); (i< 5); (i++)) {
            sscanf(&(registro[posicao]), DELIMITACAO, campo);
            posicao += (strlen(campo) + 1);

            if (i== 0) printf("\n\nCodigo: %s", campo);
            else if (i== 1) printf("\nTitulo: %s", campo);      /* Impressão dos campos da referência */
            else if (i== 2) printf("\nAutor: %s", campo);
            else if (i== 3) printf("\nAno de Publicacao: %s", campo);
            else if (i== 4) printf("\nVeiculo: %s", campo);
        }
        printf("\n\n");
    }
    return byteOffset;
}

long pesquisaArquivo(FILE *indice, ArvoreB *pagEncontrada, int codigo, int *codigoPai) {
    int flag= 0, i;
    long raiz;
    ArvoreB pagina;

    fseek(indice, 0, SEEK_SET);
    fread(&raiz, CARQTDE, 1, indice);

    while((raiz!= -1) && (ftell(indice)!= 0)) {
        fseek(indice, raiz, SEEK_SET);
        pagina.pagEndereco= raiz;

        if (flag) {
            for ((i= 0); (i< PAGTAM); (i++)) if (pagina.codigoReg[i]!= 0) {
                (*codigoPai)= pagina.codigoReg[i];
                i= PAGTAM;
            }
        }
        for ((i= 0); (i< PAGTAM); (i++)) {
            fread(&(pagina.offsetPagina[i]), CARQTDE, 1, indice);    /* Cada página é devidamente processada */
            fread(&(pagina.codigoReg[i]), CARQTDE, 1, indice);
            fread(&(pagina.offsetReg[i]), CARQTDE, 1, indice);
        }
        fread(&(pagina.offsetPagina[i]), CARQTDE, 1, indice);

        (*pagEncontrada)= pagina;
        flag= 1;

        for ((i= 0); (i< PAGTAM); (i++)) {
            if (codigo< pagina.codigoReg[i]) {   /* Conforme organização da Árvore-B, obtém-se o endereço da próxima página */
                raiz= pagina.offsetPagina[i];
                i= PAGTAM;
            }                                    /* Se o elemento foi encontrado, há o retorno de seu endereço */
            else if (codigo== pagina.codigoReg[i]) return pagina.offsetReg[i];
        }
        if (codigo> pagina.codigoReg[PAGTAM-1]) raiz= pagina.offsetPagina[PAGTAM];
    }
    return -1;
}

void finalizaExecucao(FILE *indice, FILE *dados) {
    fclose(indice);
    fclose(dados);                     /* Arquivos de dados e índices são fechados com a finalização do programa */
}
