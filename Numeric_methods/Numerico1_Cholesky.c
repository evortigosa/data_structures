/* Evandro Scudeleti Ortigossa      nUSP 6793135
   SME0100 - Cálculo Numérico I
   Exercício de Implementação: Decomposição de Cholesky */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int IsNumber(double);               /* Testa se é um número indefinido */

int main(void) {
    int n, i, j, k;
    double **A, *b, **G, **Gt, *y, *x, auxiliar;

    scanf("%d", &n);                /* Entrada da dimensão da matriz A */

    A= (double **)malloc(n* sizeof(double *));
    G= (double **)malloc(n* sizeof(double *));
    Gt= (double **)malloc(n* sizeof(double *));

    for ((i= 0); (i< n); (i++)) {
        A[i]= (double *)malloc(n* sizeof(double));
        G[i]= (double *)malloc(n* sizeof(double));
        Gt[i]= (double *)malloc(n* sizeof(double));
    }

    b= (double *)malloc(n* sizeof(double));
    y= (double *)malloc(n* sizeof(double));
    x= (double *)malloc(n* sizeof(double));

    for ((i= 0); (i< n); (i++)) {
        for ((j= i); (j< n); (j++)) {
            scanf("%lf", &A[i][j]);         /* Entrada da matriz A */

            A[j][i]= A[i][j];

            G[i][j]= 0;
            G[j][i]= 0;
            Gt[i][j]= 0;
            Gt[j][i]= 0;
        }
        y[i]= 0;
        x[i]= 0;
    }

    for ((i= 0); (i< n); (i++)) scanf("%lf", &b[i]);  /* Entrada do vetor b */

    G[0][0]= sqrt(A[0][0]);

    for ((i= 1); (i< n); (i++)) {           /* Definição da primeira coluna de G */
        G[i][0]= A[i][0]/ G[0][0];
    }

    for ((i= 1); (i< n); (i++)) {           /* Decomposição de Cholesky em G */
        for ((j= 1); (j< n); (j++)) {
            auxiliar= 0;                    /* Elementos de G são encontrados por coluna */

            if (j== i) {
                for ((k= 0); (k< j); (k++)) auxiliar += (pow(G[j][k], 2));

                G[j][j]= sqrt(A[j][j]- auxiliar);
            }
            else if (j>= i) {
                for ((k= 0); (k< i); (k++)) auxiliar += (G[j][k]* G[i][k]);

                G[j][i]= (A[j][i]- auxiliar)/ G[i][i];
            }
        }
    }

    for ((i= 0); (i< n); (i++)) {
        for ((j= i); (j< n); (j++)) Gt[i][j]= G[j][i];     /* Construção da matriz Gt */
    }

    y[0]= b[0]/ G[0][0];

    for ((i= 1); (i< n); (i++)) {           /* Gy = b */
        auxiliar= 0;

        for ((j= 0); (j< i); (j++)) auxiliar += (G[i][j]* y[j]);

        y[i]= (b[i]- auxiliar)/ G[i][i];
    }

    x[n- 1]= y[n- 1]/ Gt[n- 1][n- 1];

    for ((i= (n- 2)); (i>= 0); (i--)) {     /* Gtx = y */
        auxiliar= 0;

        for ((j= (n- 1)); (j> i); (j--)) auxiliar += (Gt[i][j]* x[j]);

        x[i]= (y[i]- auxiliar)/ Gt[i][i];
    }
    printf("\n");

    for ((i= 0); (i< n); (i++)) {
        if (!IsNumber(x[i])) {              /* Testa se foi possível resolver o sistema */
            printf("erro\n");
            return 0;
        }
    }

    for ((i= 0); (i< n); (i++)) {           /* Impressão da matriz G */
        for ((j= 0); (j< n); (j++)) {
            printf("%.4f ", G[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    for ((i= 0); (i< n); (i++)) {           /* Impressão do vetor resposta x */
        if (fabs(x[i])< 1.0e-10) printf("%.4f ", fabs(x[i]));
        else printf("%.4f ", x[i]);
    }
    printf("\n");

return 0;

}

int IsNumber(double x) {
    return (x== x);         /* It should always be true, but it's false if x is a NaN  */
}
