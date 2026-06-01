/* Evandro Scudeleti Ortigossa      nUSP 6793135
   SME0100 - Cálculo Numérico I
   Exercício de Implementação: Método da Bissecção */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 100

double calculaPx(double *, double, int);    /* Calcula o valor de um polinômio dado um ponto */

int main(void) {
    int n, i, flag= 1;
    double *Px, a, b, xk, Pa, Pb, Pxk, xkAntes= 0, tolerance;

    scanf("%d", &n);                   /* Entrada do grau do polinômio Px */

    Px= (double *)malloc((n+ 1)* sizeof(double));

    for ((i= 0); (i<= n); (i++)) scanf("%lf", &Px[i]);     /* Entrada dos coeficientes de Px */

    scanf("%lf %lf %lf", &a, &b, &tolerance);

    for ((i= 0); (i<= MAX); (i++)) {
        xk= (a+ b)/ 2;                 /* Calcula o ponto médio do intervalo */

        Pa= calculaPx(Px, a, n);
		Pxk= calculaPx(Px, xk, n);     /* Calcula o valor da função em a, b e no ponto médio */
		Pb= calculaPx(Px, b, n);

		if ((Pa* Pxk)< 0) b= xk;            /* Redefine o intervalo de acordo com os */
		else if ((Pb* Pxk)< 0) a= xk;       /* produtos f(a) com f(x) e f(b) com f(x) */
		else {
            i= (MAX+ 10);
            flag= 0;
		}

       if ((fabs(xk- xkAntes)/ fabs(xk))< tolerance) i= (MAX+ 10);   /* Condição de parada */

       xkAntes= xk;
    }

    if (i== (MAX+ 1)) printf("maximo de iteracoes\n");

    if (flag) printf("%.4f\n", xk);         /* Impressão da resposta final */
    else printf("erro\n");

return 0;

}

double calculaPx(double *Px, double x, int n) {
    int i;
    double valor= 0;

    for ((i= 0); (i<= n); (i++)) valor += (Px[i]* pow(x, i));

    return valor;       /* Retorna o valor do um polinômio para o ponto dado */

}
