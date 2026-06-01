/* Evandro Scudeleti Ortigossa      nUSP 6793135
   SME0100 - Cálculo Numérico I
   Exercício de Implementação: Método de Briot-Ruffini-Horner */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 50

int main(void) {
    int n, i, iteracoes= 0, flag= 1;
    double *Px, bn, cn, xk, xkAntes= 0, tolerance;

    scanf("%d", &n);                   /* Entrada do grau do polinômio Px */

    Px= (double *)malloc((n+ 1)* sizeof(double));

    for ((i= 0); (i<= n); (i++)) scanf("%lf", &Px[i]);     /* Entrada dos coeficientes de Px */

    scanf("%lf %lf", &xk, &tolerance);

    do {
        bn= Px[n];           /* Inicialização de bn e cn a cada iteração do método */
        cn= bn;

        for ((i= 1); (i<= n); (i++)) {      /* Método de Briot-Ruffini-Horner */

            if (i> 1) cn= ((xk* cn)+ bn);   /* cn-k = xcn-k+1 + bn-k */

            bn= ((xk* bn)+ Px[n- i]);       /* bn-k = xbn-k+1 + an-k */
        }

        xk= (xk- (bn/ cn));       /* Iteração do Método de Newton */

        if (fabs((xk- xkAntes)/ xk)< tolerance) flag= 0;   /* Condição de parada */

       xkAntes= xk;
       iteracoes++;

    } while ((iteracoes< MAX) && (flag));

    if (iteracoes== MAX) printf("maximo de iteracoes\n");

    printf("%.4f\n", xk);         /* Impressão da resposta final */

return 0;

}
