#include <stdio.h>
#include <omp.h>

#define N 10
#define M 1000

void multiplicacao(int *vetor, int inicio, int fim, int k);
void imprime(int *vetor, int inicio, int fim);

int main (){
	omp_set_num_threads(N);
	int i, vetor[M], k;
	int inicio = 0, fim = 99;
	
	for (i=0;i<M;i++)
		vetor[i] = rand() % 1000 + 1;
	
	imprime(vetor,0,M-1);
	
	printf ("\n\nInsira um escalar para multiplicacao do vetor: ");
	scanf ("%d", &k);
	
	#pragma omp parallel
	{
		#pragma omp for
		for (i=0;i<N;i++)
		{
			multiplicacao(vetor,inicio,fim, k);
			inicio=fim+1;
			fim+=100;	
		}
	}
	
	printf ("\n\n");
	imprime(vetor,0,M-1);
	
	
	return 0;
}


void multiplicacao(int *vetor, int inicio, int fim, int k){
	int i;	
	for (i=inicio;i<=fim;i++)
		vetor[i] *= k;
}

void imprime(int *vetor, int inicio, int fim){
	int i;
	for (i=inicio;i<=fim;i++){
		printf ("%d  ",vetor[i]);
	}
}
