#include <stdio.h>

unsigned long int fat (int x)
{
   unsigned long int f=1;
   int i;

   if (x<0){
      fprintf(stderr, "Erro. Fatorial de negativo n�o existe!\n");
      return(0);
   }

   for (i = 2; i <= x; i++)
      f = f*i;
   /* se x for 0 ou 1, n�o entrara no la�o, retornando 1. Certo, pois fat(0) = 1 e fat(1) = 1 */
   return(f);
}

int main()
{
   int x;

   printf("Tamanho da minha ULA � %d\n", sizeof(long int));

   printf("Digite um n�mero positivo: ");
   scanf("%d", &x);
   printf("Fatorial de %d = %lu\n", x, fat(x));
}
