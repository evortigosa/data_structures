/*  Algoritmos e Estruturas de Dados II - Trabalho 2 - Árvores-B (prototype)
          Evandro Scudeleti Ortigossa, n° USP 6793135
          Bacharelado em Ciências de Computação 2009           */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <limits.h>
#include <errno.h>

#define REGTAM 256                          /* Definição das principais constantes utilizadas no sistema */
#define PAGTAM 4
#define CARQTDE sizeof(long)
#define LONGTAM sizeof(long)
#define INTTAM  sizeof(int)
#define PAGBYTE ((PAGTAM + 1) * LONGTAM + PAGTAM * INTTAM + PAGTAM * LONGTAM)
#define DELIMITADOR '@'
#define DELIMITACAO "%255[^@]"
#define SCFDELIMITACAO "%5[^@]"
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
int leLinha(char *, size_t);
void processaComando(int *);
char *duplicaString(const char *);
void recebeReferencia(FILE *, FILE *, long);
int lePagina(FILE *, long, ArvoreB *);
int gravaPagina(FILE *, ArvoreB );
int calculaCodigo(const char *);
void imprimeReferencia(FILE *, FILE *, Referencia, long);
void imprimeIndice(FILE *, ArvoreB);
void inicializaIndice(FILE *, int, long);
void inicializaPagina(ArvoreB *, long);
int paginaCheia(ArvoreB);
void insereOrdenado(FILE *, ArvoreB *, int, long, long);
void splitDeRaiz(FILE *, ArvoreB *, int, long, long);
long split(FILE *, ArvoreB *, int *, long *, long);
long pop(FILE *);
void insereIndice(FILE *, int, long, long, long);
void alteraReferencia(FILE *, FILE *);
void removeReferencia(FILE *, FILE *);
int verificaFolha(ArvoreB);
void push(FILE *, ArvoreB);
int contaChaves(ArvoreB);
int removeOrdenado(ArvoreB *, int);
int redistribuicao(FILE *, ArvoreB *, long, int *);
int primeiraChave(ArvoreB);
long buscaPaiDaPagina(FILE *, ArvoreB);
int concatenacao(FILE *, ArvoreB *, long *, int, long);
int buscaMenor(FILE *, ArvoreB *, int *, long *);
void removeIndice(FILE *, ArvoreB, int, long);
void reorganizaArvore(FILE *, ArvoreB *, long, int, long);
long buscaReferecia(FILE *, FILE *);
long pesquisaArquivo(FILE *, ArvoreB *, int, long *);
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

int leLinha(char *buffer, size_t tamanho) {
    /* Lê entrada evitando que caracteres excedentes ao buffer permanecam no stdin */
    int c;
    size_t len;

    if (fgets(buffer, tamanho, stdin)== NULL) return 0;
    len= strlen(buffer);

    if ((len> 0) && (buffer[len-1]== '\n')) {
        buffer[len-1]= '\0';
    }
    else {
        while ((c= getchar())!= '\n' && c!= EOF) {
            /* descarta o restante da linha */
        }
    }
    return 1;
}

void processaComando(int *opcao) {
    char comando[20]= " ";

    leLinha(comando, sizeof comando);

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

char *duplicaString(const char *s) {
    char *copia= malloc(strlen(s) + 1);
    if (copia== NULL) {
        fprintf(stderr, "Erro de memoria.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(copia, s);
    return copia;
}

void recebeReferencia(FILE *indice, FILE *dados, long posicao) {
    char auxChar[REGTAM+ 1]= " ";
    Referencia registro;

    if (posicao== -1) {
        printf("Codigo: ");          
        leLinha(registro.codigo, sizeof registro.codigo);    /* Respectivos dados de uma referência são captados */
    }
    else {
        fseek(dados, posicao, SEEK_SET);
        if (fread(auxChar, 1, REGTAM, dados)!= REGTAM) {
        printf("\nErro ao ler registro.\n");
            return;
        }
        auxChar[REGTAM]= '\0';
        sscanf(auxChar, SCFDELIMITACAO, registro.codigo);
    }
    printf("Titulo: ");
    leLinha(auxChar, sizeof auxChar);
    registro.titulo= duplicaString(auxChar);
    printf("Autor: ");
    leLinha(auxChar, sizeof auxChar);
    registro.autor= duplicaString(auxChar);
    printf("Ano de Publicacao: ");
    leLinha(auxChar, sizeof auxChar);
    registro.anoPublicacao= duplicaString(auxChar);
    printf("Veiculo: ");
    leLinha(auxChar, sizeof auxChar);
    registro.veiculo= duplicaString(auxChar);
    printf("\n");

    imprimeReferencia(indice, dados, registro, posicao);

    free(registro.titulo);
    free(registro.autor);
    free(registro.anoPublicacao);                /* Liberação de memória alocada pelo registro de referências */
    free(registro.veiculo);
}

int lePagina(FILE *indice, long endereco, ArvoreB *pagina) {
    int i;

    if (endereco< 0) return 0;
    if (fseek(indice, endereco, SEEK_SET)!= 0) return 0;

    pagina->pagEndereco= endereco;

    for ((i= 0); (i< PAGTAM); (i++)) {
        if (fread(&pagina->offsetPagina[i], sizeof pagina->offsetPagina[i], 1, indice)!= 1) return 0;
        if (fread(&pagina->codigoReg[i],    sizeof pagina->codigoReg[i],    1, indice)!= 1) return 0;
        if (fread(&pagina->offsetReg[i],    sizeof pagina->offsetReg[i],    1, indice)!= 1) return 0;
    }
    if (fread(&pagina->offsetPagina[i], sizeof pagina->offsetPagina[i], 1, indice)!= 1) return 0;
    return 1;
}

int gravaPagina(FILE *indice, ArvoreB pagina) {
    int i;

    if (pagina.pagEndereco== -1) {
        if (fseek(indice, 0, SEEK_END) != 0) return 0;
        pagina.pagEndereco= ftell(indice);
    } else {
        if (fseek(indice, pagina.pagEndereco, SEEK_SET)!= 0) return 0;
    }

    for ((i= 0); (i< PAGTAM); (i++)) {      /* Impressão de uma página completa da Árvore-B */
        if (fwrite(&pagina.offsetPagina[i], sizeof pagina.offsetPagina[i], 1, indice)!= 1) return 0;
        if (fwrite(&pagina.codigoReg[i],    sizeof pagina.codigoReg[i],    1, indice)!= 1) return 0;
        if (fwrite(&pagina.offsetReg[i],    sizeof pagina.offsetReg[i],    1, indice)!= 1) return 0;
    }

    if (fwrite(&pagina.offsetPagina[i], sizeof pagina.offsetPagina[i], 1, indice)!= 1) return 0;
    return 1;
}

int calculaCodigo(const char *texto) {
    char *fim;
    long valor;

    errno = 0;
    valor = strtol(texto, &fim, 10);

    if (texto[0] == '\0' || *fim != '\0' || errno != 0 || valor <= 0 || valor > INT_MAX) {
        return -1;
    }

    return (int)valor;    /* Valor numérico do código */
}

void imprimeReferencia(FILE *indice, FILE *dados, Referencia registro, long posicao) {
    int tamanho, codigo= 0, i;
    long byteOffset= 0, enderecoPai= -1;
    ArvoreB paginaExistente;
    /* delimitadores */
    tamanho= strlen(registro.codigo)+ strlen(registro.titulo)+ strlen(registro.autor)+ strlen(registro.anoPublicacao)+ strlen(registro.veiculo) + 5;

    if (tamanho> REGTAM) {
        printf("\nRegistro muito grande. Operacao cancelada.\n");
        return;
    }
    if (posicao== -1) {
        codigo= calculaCodigo(registro.codigo);
        if (codigo< 0) {
            printf("\nCodigo invalido.\n");
            return;
        }
        if (pesquisaArquivo(indice, &paginaExistente, codigo, &enderecoPai)>= 0) {
            printf("\nCodigo ja existente. Insercao cancelada.\n");
            return;
        }
        fseek(dados, 0, SEEK_END);
        byteOffset= ftell(dados);
    } else {
        fseek(dados, posicao, SEEK_SET);
    }
    fprintf(dados, "%s%c%s%c%s%c%s%c%s%c", 
            registro.codigo, DELIMITADOR, registro.titulo, DELIMITADOR, registro.autor, DELIMITADOR, registro.anoPublicacao, DELIMITADOR, registro.veiculo, DELIMITADOR);

    for ((i= tamanho); (i< REGTAM); (i++)) {
        fputc(PREENCHE, dados);
    }
    if (posicao== -1) {
        insereIndice(indice, codigo, byteOffset, -1, codigo);
    }
}

void imprimeIndice(FILE *indice, ArvoreB pagina) {
    gravaPagina(indice, pagina);
}

void inicializaIndice(FILE *indice, int codigo, long posRegistro) {
    long topoPilha= -1;
    long raiz= 2*sizeof(long);
    ArvoreB pagina;

    fwrite(&raiz, sizeof raiz, 1, indice);              /* Inicialização do arquivo de índices com os dados do primeiro */
    fwrite(&topoPilha, sizeof topoPilha, 1, indice);    /* registro inserido no arquivo de dados */
    inicializaPagina(&pagina, -1);
    pagina.codigoReg[PAGTAM-1]= codigo;
    pagina.offsetReg[PAGTAM-1]= posRegistro;

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

void insereIndice(FILE *indice, int codigo, long posRegistro, long ponteiro, long localizacao) {
    long posicao, enderecoPai= -1;
    ArvoreB pagina;

    fseek(indice, 0, SEEK_END);

    if (ftell(indice)== 0) {
        /* Caso de inserção dos primeiros dados no arquivo de índice */
        inicializaIndice(indice, codigo, posRegistro);
        return;
    }

    if (ponteiro== -1) {
        /* Inserção normal: localizar a folha pelo código. */
        pesquisaArquivo(indice, &pagina, codigo, &enderecoPai);
    }
    else {
        /* Inserção recursiva após split: localizacao é o endereço da página pai. */
        if (!lePagina(indice, localizacao, &pagina)) {
            printf("\nErro ao localizar pagina pai durante insercao.\n");
            return;
        }
        enderecoPai= buscaPaiDaPagina(indice, pagina);
    }

    if (!paginaCheia(pagina)) {
        insereOrdenado(indice, &pagina, codigo, posRegistro, ponteiro);
    }
    else {                                        /* Caso em que há espaço na página que o dado deve ser inserido */
        fseek(indice, 0, SEEK_SET);
        fread(&posicao, CARQTDE, 1, indice);      /* Senão há a necessidade de split de página ou de raiz */

        if (pagina.pagEndereco== posicao) {
            splitDeRaiz(indice, &pagina, codigo, posRegistro, ponteiro);
        }
        else {
            posicao= split(indice, &pagina, &codigo, &posRegistro, ponteiro);
            insereIndice(indice, codigo, posRegistro, posicao, enderecoPai);
        }
    }
}

void alteraReferencia(FILE *indice, FILE *dados) {
    long byteOffset;

    byteOffset= buscaReferecia(indice, dados);        /* Se a referência existir, será alterada pelo usuário */

    if (byteOffset!= -1) recebeReferencia(indice, dados, byteOffset);
}

void removeReferencia(FILE *indice, FILE *dados) {
    int codigo= 0;
    long byteOffset, enderecoPai= -1;
    char codChar[6]= " ";
    ArvoreB pagina;

    printf("Insira o codigo da referencia: ");
    leLinha(codChar, sizeof codChar);
    codigo= calculaCodigo(codChar);
    if (codigo < 0) {
        printf("\nCodigo invalido.\n");
        return;
    }
    byteOffset= pesquisaArquivo(indice, &pagina, codigo, &enderecoPai);     /* Pesquisa pelo código da referência no arquivo de índices */

    if (byteOffset< 0) printf("\n\nReferencia inexistente!!!\n\n");         /* Retorno de seu respectivo byteoffset, se -1, arquivo não existe */
    else {
        fseek(dados, byteOffset, SEEK_SET);
        fprintf(dados, "%s", EXCLUIDO);
        removeIndice(indice, pagina, codigo, enderecoPai);                  /* Exclusão no arquivo de dados e do respectivo índice */
        printf("\n\nReferencia removida com sucesso!!!\n\n");
    }
}

int verificaFolha(ArvoreB pagina) {
    int i;

    for ((i= 0); (i<= PAGTAM); (i++)) {
        if (pagina.offsetPagina[i]!= -1) return 0;
    }
    return 1;      /* Páginas são mantidas em ordem, logo se pode verificar a existência de filhos na última posição do vetor */
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

int contaChaves(ArvoreB pagina) {
    int i, n= 0;

    for ((i= 0); (i< PAGTAM); (i++)) {
        if (pagina.codigoReg[i]!= 0) n++;
    }
    return n;
}

int removeOrdenado(ArvoreB *pagina, int codigo) {
    int posRemocao= -1, posVazias= 1, i;

    for ((i= 0); ((i< PAGTAM) && (posRemocao== -1)); (i++)) {
        if ((*pagina).codigoReg[i]== codigo) posRemocao= i;
        else if ((*pagina).codigoReg[i]== 0) posVazias++;
    }
    if (posRemocao == -1) {
        return 0;
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

    return contaChaves(*pagina) < (PAGTAM / 2);
}

int redistribuicao(FILE *indice, ArvoreB *pagina, long enderecoPai, int *posOffSet) {
    /*
        TODO: internal-node underflow is still not fully handled. 
    */
    int posVizVazias, posPagVazias, posRemocao, sucesso= 0, i, j;
    ArvoreB paginaPai, paginaDireita, paginaEsquerda;

    if (!lePagina(indice, enderecoPai, &paginaPai)) {
        return 0;
    }

    for ((i= 0); (i<= PAGTAM); (i++)) {
        if (paginaPai.offsetPagina[i]== (*pagina).pagEndereco) {
            (*posOffSet)= i;
            posPagVazias= PAGTAM-(PAGTAM/2);     /* Neste caso, se possível, os elementos da página com underflow são */
                                                 /* redistribuídos com os da página irmã direita */
            if ((i< PAGTAM) && (paginaPai.offsetPagina[i+1]!= -1)) {
                if (!lePagina(indice, paginaPai.offsetPagina[i+1], &paginaDireita)) {
                    return 0;
                }
                posVizVazias= 0, posRemocao= -1;

                for ((j= 0); (j< PAGTAM); (j++)) {
                    if (paginaDireita.codigoReg[j]== 0) posVizVazias++;
                    if ((paginaDireita.codigoReg[j]!= 0) && (posRemocao== -1)) posRemocao= j;
                }

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
                if (!lePagina(indice, paginaPai.offsetPagina[i-1], &paginaEsquerda)) {
                    return 0;
                }
                posVizVazias= 0;    /* Neste caso, se não houve redistribuição com a página irmã direita, se possível, */
                                   /* os elementos da página com underflow são redistribuídos com os da página irmã esquerda */
                for ((j= 0); (j< PAGTAM); (j++)) {
                    if (paginaEsquerda.codigoReg[j]== 0) posVizVazias++;
                }

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

int primeiraChave(ArvoreB pagina) {
    int i;

    for ((i= 0); (i< PAGTAM); (i++)) {
        if (pagina.codigoReg[i]!= 0) {
            return pagina.codigoReg[i];
        }
    }
    return 0;
}

long buscaPaiDaPagina(FILE *indice, ArvoreB pagina) {
    int chave;
    long enderecoPai= -1;
    ArvoreB aux;

    chave= primeiraChave(pagina);
    if (chave== 0) {
        return -1;
    }
    pesquisaArquivo(indice, &aux, chave, &enderecoPai);

    return enderecoPai;
}

int concatenacao(FILE *indice, ArvoreB *pagina, long *enderecoPai, int posOffSet, long raiz) {
    int posVizVazias, sucesso= 0, underFlow= 0, i;
    long enderecoAvo;
    ArvoreB paginaPai, auxPagina;

    if (!lePagina(indice, *enderecoPai, &paginaPai)) {
        return 0;
    }
    enderecoAvo= buscaPaiDaPagina(indice, paginaPai);
    (*enderecoPai)= enderecoAvo;

    if ((posOffSet< PAGTAM) && (paginaPai.offsetPagina[posOffSet+1]!= -1)) {
        if (!lePagina(indice, paginaPai.offsetPagina[posOffSet+1], &auxPagina)) {
            return 0;
        }
        posVizVazias= 0;    /* Neste caso os elementos da página em underflow são concatenados com os da página irmã direita */
        
        for ((i= 0); (i< PAGTAM); (i++)) {
            if (auxPagina.codigoReg[i]== 0) posVizVazias++;
        }

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
                underFlow= 0;
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
        if (!lePagina(indice, paginaPai.offsetPagina[posOffSet-1], &auxPagina)) {
            return 0;
        }
        posVizVazias= 0;    /* Neste caso, se a concatenação com a página irmã direita não foi bem sucedida, os */
                            /* elementos da página em underflow são concatenados com os da página irmã esquerda */
        for ((i= 0); (i< PAGTAM); (i++)) {
            if (auxPagina.codigoReg[i]== 0) posVizVazias++;
        }

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
                underFlow= 0;
                push(indice, paginaPai);
                fseek(indice, 0, SEEK_SET);      /* Altura da árvore é reduzida: exclusão e empilhamento da antiga raiz */
                fwrite(&(auxPagina.pagEndereco), CARQTDE, 1, indice);
            }
            else {
                paginaPai.offsetPagina[posOffSet]= auxPagina.pagEndereco;
                imprimeIndice(indice, paginaPai);
            }
            sucesso= 1;    /* Operação bem sucedida */
        }
    }
    if (!sucesso) {
        return underFlow;
    }
    push(indice, (*pagina));           /* Página concatenada é excluída e empilhada */
    imprimeIndice(indice, auxPagina);
    (*pagina)= paginaPai;    /* Note que quem retorna é a página "pai" da página concatenada: tratamento de underflow propagado */

    return underFlow;        /* Retorno de informação sobre propagação de underflow */
}

int buscaMenor(FILE *indice, ArvoreB *pagina, int *codigo, long *enderecoPai) {
    int posTroca= 0, primeiro, i;
    long posPagina= 0, paiAtual, auxOffsetReg= 0;
    ArvoreB auxPagina;

    for ((i= (PAGTAM-1)); (i>= 0); (i--)) {
        if ((*pagina).codigoReg[i]== (*codigo)) {
            posTroca= i;
            posPagina= (*pagina).offsetPagina[i+1];    /* Busca pela posição, na página interna, do elemento a ser excluído */
            break;
        }
    }
    paiAtual= (*pagina).pagEndereco;

    do {
        if (!lePagina(indice, posPagina, &auxPagina)) return 0;    /* Busca pela folha que contém o elemento adequado para troca com o elemento */
        primeiro= -1;                                              /* a ser excluído na página interna: menor elemento da página mais a esquerda */

        for ((i= 0); (i< PAGTAM); (i++)) {
            if (auxPagina.codigoReg[i]!= 0) {                      /* Processamento das páginas até encontrar a adequada */
                primeiro= i;
                break;
            }
        }
        if (primeiro== -1) return 0;

        (*codigo)= auxPagina.codigoReg[primeiro];                  /* Armazenamento do menor elemento da página encontrada */
        auxOffsetReg= auxPagina.offsetReg[primeiro];

        if (!verificaFolha(auxPagina)) {
            paiAtual= auxPagina.pagEndereco;
            posPagina= auxPagina.offsetPagina[primeiro];
        }

    } while (!verificaFolha(auxPagina));

    (*enderecoPai)= paiAtual;
    (*pagina).codigoReg[posTroca]= (*codigo);    /* Troca com o menor elemento da página mais a esquerda: "exclusão" */
    (*pagina).offsetReg[posTroca]= auxOffsetReg;
    imprimeIndice(indice, (*pagina));
    (*pagina)= auxPagina;              /* Note que quem volta para a função remoção é a página folha e o elemento usado */
                                       /* como troca, seguindo a regra de só realizar verdadeiras exclusões em folhas */
    return 1;
}

void removeIndice(FILE *indice, ArvoreB pagina, int codigo, long enderecoPai) {
    /*
        TODO: partially supported; improve it when deletion causes underflow.
    */
    int underFlow= 0;
    long raiz;

    fseek(indice, 0, SEEK_SET);
    fread(&raiz, CARQTDE, 1, indice);
                                       /* Exclusão de elemento em página interna, busca de um substituto em folha */
    if (!verificaFolha(pagina)) {
        if (!buscaMenor(indice, &pagina, &codigo, &enderecoPai)) {
            printf("\nErro ao buscar substituto para remocao.\n");
            return;
        }
    }
                                       /* Note que a página anteriormente interna volta como folha */
    underFlow= removeOrdenado(&pagina, codigo);
                                       /* Se houver underflow, necessidade de reorganização */
    if ((underFlow) && (pagina.pagEndereco!= raiz)) reorganizaArvore(indice, &pagina, enderecoPai, underFlow, raiz);
    else imprimeIndice(indice, pagina);
}

void reorganizaArvore(FILE *indice, ArvoreB *pagina, long enderecoPai, int underFlow, long raiz) {
    int redistribuiu= 1, posOffSet= 0;

    redistribuiu= redistribuicao(indice, pagina, enderecoPai, &posOffSet);
                                       /* Redistribuição pode ou não ser aplicada, se não: concatenação */
    if (!redistribuiu) {
        underFlow= concatenacao(indice, pagina, &enderecoPai, posOffSet, raiz);
        if (underFlow) reorganizaArvore(indice, pagina, enderecoPai, underFlow, raiz);
    }                                  /* Se o underflow se propagou, volto recursivamente reorganizando as páginas "pai" */
}

long buscaReferecia(FILE *indice, FILE *dados) {
    int codigo= 0, posicao= 0, i;
    long byteOffset, enderecoPai= -1;
    char codChar[6]= " ", registro[REGTAM+ 1]= " ", campo[REGTAM]= " ";
    ArvoreB pagina;

    printf("Insira o codigo da referencia: ");
    leLinha(codChar, sizeof codChar);
    codigo= calculaCodigo(codChar);
    if (codigo < 0) {
        printf("\nCodigo invalido.\n");
        return -1;
    }
    byteOffset= pesquisaArquivo(indice, &pagina, codigo, &enderecoPai);

    if (byteOffset< 0) printf("\n\nReferencia inexistente!!!\n\n");
    else {
        fseek(dados, byteOffset, SEEK_SET);      /* Após pesquisa bem sucedida, os campos do registro são separados pelos */
        if (fread(registro, 1, REGTAM, dados)!= REGTAM) {
            printf("\nErro ao ler registro.\n");
            return -1;
        }
        registro[REGTAM]= '\0';                  /* delimitadores de campo e impressos para o usuário */

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

long pesquisaArquivo(FILE *indice, ArvoreB *pagEncontrada, int codigo, long *enderecoPai) {
    int i, desceu;
    long atual, pai= -1;
    ArvoreB pagina;

    if (enderecoPai!= NULL) (*enderecoPai)= -1;

    fseek(indice, 0, SEEK_END);
    if (ftell(indice)== 0) return -1;

    fseek(indice, 0, SEEK_SET);
    if (fread(&atual, sizeof atual, 1, indice)!= 1) return -1;

    while (atual!= -1) {
        if (!lePagina(indice, atual, &pagina)) return -1;

        if (pagEncontrada!= NULL) (*pagEncontrada)= pagina;

        if (enderecoPai!= NULL) (*enderecoPai)= pai;

        desceu= 0;

        for ((i= 0); (i< PAGTAM); (i++)) {    /* Cada página é devidamente processada */
            if (pagina.codigoReg[i]!= 0) {
                if (codigo== pagina.codigoReg[i]) return pagina.offsetReg[i];  /* Se o elemento foi encontrado, há o retorno de seu endereço */

                if (codigo< pagina.codigoReg[i]) {    /* Conforme organização da Árvore-B, obtém-se o endereço da próxima página */
                    pai= atual;
                    atual= pagina.offsetPagina[i];
                    desceu= 1;
                    break;
                }
            }
        }
        if (!desceu) {
            pai= atual;
            atual= pagina.offsetPagina[PAGTAM];
        }
    }
    return -1;
}

void finalizaExecucao(FILE *indice, FILE *dados) {
    fclose(indice);
    fclose(dados);                     /* Arquivos de dados e índices são fechados com a finalização do programa */
}
