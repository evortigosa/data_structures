/* Evandro Scudeleti Ortigossa      nUSP 6793135
   SME0100 - Cálculo Numérico I
   Exercício de Implementação: Método de Leverrier */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void) {
    int n, i, j, k, l;
    double **A, **Ak, **Aux, *sk, *pk, *pn;

    scanf("%d", &n);                   /* Entrada da dimensão da matriz A */

    A= (double **)malloc(n* sizeof(double *));
    Ak= (double **)malloc(n* sizeof(double *));
    Aux= (double **)malloc(n* sizeof(double *));

    for ((i= 0); (i< n); (i++)) {
        A[i]= (double *)malloc(n* sizeof(double));
        Ak[i]= (double *)malloc(n* sizeof(double));
        Aux[i]= (double *)malloc(n* sizeof(double));
    }

    sk= (double *)malloc(n* sizeof(double));
    pk= (double *)malloc(n* sizeof(double));
    pn= (double *)malloc((n+ 1)* sizeof(double));

    for ((i= 0); (i< n); (i++)) {
        for ((j= 0); (j< n); (j++)) {
            scanf("%lf", &A[i][j]);         /* Entrada da matriz A */

            Ak[i][j]= A[i][j];
        }
        sk[i]= 0;
        pk[i]= 0;
        pn[i]= 0;
    }

    for ((i= 0); (i< n); (i++)) {
        for ((j= 0); (j< n); (j++)) sk[i] += Ak[j][j];     /* Cálculo dos sk */

        for ((j= 0); (j< n); (j++)) {
            for ((k= 0); (k< n); (k++)) {                  /* Multiplicação de Ak por A */
                Aux[j][k]= 0;

                for ((l= 0); (l< n); (l++)) Aux[j][k] += (Ak[j][l]* A[l][k]);
            }
        }

        for ((j= 0); (j< n); (j++)) {                      /* Cópia de Aux para Ak */
            for ((k= 0); (k< n); (k++)) Ak[j][k]= Aux[j][k];
        }

        pk[i]= sk[i];                                      /* Cálculo dos pk */

        for ((j= 0); (j< i); (j++)) pk[i] -= (pk[j]* sk[(i- 1)- j]);

        pk[i] /= (i+ 1);
    }

    for ((i= 0); (i< n); (i++)) {                          /* Cálculo do polinômio característico pn */
        pn[i]= (pow((-1), n)* (-1)* pk[(n- 1)- i]);
    }
    pn[n]= pow((-1), n);

    printf("\n");

    for ((i= 0); (i<= n); (i++)) printf("%.4fx%d  ", pn[i], i);

return 0;

}
