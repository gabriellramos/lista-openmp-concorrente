#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>
#define QTD_MAX 6
#define NUM_EXEC 10
#define NUM_THREADS getNumThreads()


long int N, num_threads;
long int qtdElementos[QTD_MAX] = {10000, 20000, 50000, 1000000,	5000000, 150000000 };
int   qtdThreads[QTD_MAX] = {2, 4, 8, 16, 32, 64};
FILE *mainF;

int getNumThreads(){
	return num_threads;
}

void imprimePrimosSemThreads(){
	long int cont=0,i, k,j;
	
	k=0;
	for (i=1;i<N;i++){
		if (i>1){
			cont=1;
			for (j=2;cont && j<=sqrt(i);j++)
				if (i % j == 0){
					cont = 0;
					break;
				}	
		}
		
		if (cont){
			printf ("%5d\t",i);
			k++; 
			cont++;
			if (k==10){
				printf ("\n");
				k=0;
			}
		}
	}
}

int main(){
	long int i, j;
	double tempo, soma=0,aux;
	clock_t begin, end;
	mainF = fopen("mainF.txt","w");
	FILE *file = fopen("graph.txt","w");
	
	for (j=0;j<QTD_MAX; j++){
		N = qtdElementos[j];
		num_threads = qtdThreads[j];
		soma=0;	
		for (i=0;i<NUM_EXEC; i++){
			//inicializando as variáveis
			begin = clock();
			imprimePrimosSemThreads();
			end = clock();
			tempo = (double)(end-begin)/CLOCKS_PER_SEC;
			soma+=tempo;
			printf ("Execucao %2d com %ld numeros em %lf s\n",i+1,N,tempo);
		
		//	system("pause");
		}
		fprintf (file,"%d\t%lf\n",N,soma/NUM_EXEC);
	}
}

