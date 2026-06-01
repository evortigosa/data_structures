/* Evandro Scudeleti Ortigossa      nUSP 6793135
   SME0100 - Cálculo Numérico I
   Exercício de Implementação: Decomposição LU */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int IsNumber(double);               /* Testa se é um número indefinido */

int main(void) {
    int n, i, j, k;
    double **A, *b, **L, **U, *y, *x, auxiliar;

    scanf("%d", &n);                /* Entrada da dimensão da matriz A */

    A= (double **)malloc(n* sizeof(double *));
    L= (double **)malloc(n* sizeof(double *));
    U= (double **)malloc(n* sizeof(double *));

    for ((i= 0); (i< n); (i++)) {
        A[i]= (double *)malloc(n* sizeof(double));
        L[i]= (double *)malloc(n* sizeof(double));
        U[i]= (double *)malloc(n* sizeof(double));
    }

    b= (double *)malloc(n* sizeof(double));
    y= (double *)malloc(n* sizeof(double));
    x= (double *)malloc(n* sizeof(double));

    for ((i= 0); (i< n); (i++)) {
        for ((j= 0); (j< n); (j++)) {
            scanf("%lf", &A[i][j]);         /* Entrada da matriz A */

            L[i][j]= 0;
            U[i][j]= 0;
        }
        y[i]= 0;
        x[i]= 0;
    }

    for ((i= 0); (i< n); (i++)) scanf("%lf", &b[i]);  /* Entrada do vetor b */

    for ((i= 0); (i< n); (i++)) {
        L[i][0]= A[i][0]/ A[0][0];          /* Definição da primeira coluna e diagonal principal de L */
        L[i][i]= 1;

        U[0][i]= A[0][i];                   /* Definição da primeira linha de U */
    }

    for ((i= 1); (i< n); (i++)) {           /* Decomposição LU */
        for ((j= 1); (j< n); (j++)) {
            auxiliar= 0;

            if (i<= j) {
                for ((k= 0); (k< i); (k++)) auxiliar += (L[i][k]* U[k][j]);

                U[i][j]= A[i][j]- auxiliar;
            }
            else if (i> j) {
                for ((k= 0); (k< j); (k++)) auxiliar += (L[i][k]* U[k][j]);

                L[i][j]= (A[i][j]- auxiliar)/ U[j][j];
            }
        }
    }

    y[0]= b[0];

    for ((i= 1); (i< n); (i++)) {           /* Ly = b */
        auxiliar= 0;

        for ((j= 0); (j< i); (j++)) auxiliar += (L[i][j]* y[j]);

        y[i]= b[i]- auxiliar;
    }

    x[n- 1]= y[n- 1]/ U[n- 1][n- 1];

    for ((i= (n- 2)); (i>= 0); (i--)) {     /* Ux = y */
        auxiliar= 0;

        for ((j= (n- 1)); (j> i); (j--)) auxiliar += (U[i][j]* x[j]);

        x[i]= (y[i]- auxiliar)/ U[i][i];
    }
    printf("\n");

    for ((i= 0); (i< n); (i++)) {
        if (!IsNumber(x[i])) {              /* Testa se foi possível resolver o sistema */
            printf("erro\n");
            return 0;
        }
    }

    for ((i= 0); (i< n); (i++)) {           /* Impressão da matriz L */
        for ((j= 0); (j< n); (j++)) {
            printf("%.4f ", L[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    for ((i= 0); (i< n); (i++)) {           /* Impressão da matriz U */
        for ((j= 0); (j< n); (j++)) {
            printf("%.4f ", U[i][j]);
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
