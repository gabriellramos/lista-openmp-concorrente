#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>
#define QTD_MAX 6   
#define NUM_EXEC 10
#define NUM_THREADS getNumThreads()
#define INTERVALO getIntervalo()


long int N, num_threads, inter;
long int qtdElementos[QTD_MAX] = {10000, 20000, 50000, 1000000,	5000000, 10000000};
int   qtdThreads[QTD_MAX] = {2, 4, 8, 16, 32, 64};

pthread_mutex_t mutexsum;
int inicio=0, fim=999, count=0;
void imprimePrimosSemThreads();
void imprimePrimos(int tid);
void OpenMP();
void Threads();
void *imprimePrimosThreads(void *tid);

int getNumThreads(){
	return num_threads;
}

int getIntervalo(){
	return inter;
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

void imprimePrimos(int tid){
	long int cont=0,i, k,j;
	int id = tid;
	
	k=0;
//	printf ("\nOpenMP %3d - (%d a %d)\n",id, inicio, fim);
	
	for (i=inicio;i<fim;i++){
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
			if (k==10){
				printf ("\n");
				k=0;
			}
		}
	}

}

void OpenMP(){	
	long	int thread_id, i;
	//setting threads max
	omp_set_num_threads(NUM_THREADS);
	
    #pragma omp parallel private(thread_id) 
    {
        thread_id = omp_get_thread_num();
        #pragma omp for ordered
    	for (i=0;i<omp_get_num_threads(); i++)
    	{
    		#pragma omp ordered
    		imprimePrimos(thread_id);
    		inicio=fim+1;
    		fim+=inter;
		}
    }
}

void Threads(){
	pthread_mutex_init(&mutexsum, NULL);
	pthread_t threads[NUM_THREADS];
	
	long int i, status;
	for (i=0;i<NUM_THREADS;i++){
		status = pthread_create(&(threads[i]), NULL, imprimePrimosThreads, (void*)i);
		
    	inicio=fim+1;
    	fim+=inter;
	}
	
	for (i=0;i<NUM_THREADS;i++){
		pthread_join(threads[i],NULL);
	}
	pthread_mutex_destroy(&mutexsum);
}


void *imprimePrimosThreads(void *tid){
	long int cont=0,i,aux,j, k;
	int id = (int)tid;	

	pthread_mutex_lock (&mutexsum);
	//printf ("\nTHREAD %d - (%d a %d)\n ",id, inicio, fim);
	for (i=inicio;i<fim;i++){
		if (i>1){
			cont=1;
			for (j=2;cont && j<=sqrt(i);j++){
				if (i % j == 0){
					cont = 0;
					break;
				}
			}
		}
		
		if (cont){
			printf ("%5d\t",i);
			k++;
			if (k==10){
				printf ("\n");
				k=0;
			}
		}
	}
		
	pthread_mutex_unlock (&mutexsum);
	pthread_exit(NULL);
}

int main(){
	long int i, j;
	double tempo, soma=0,aux;
	clock_t begin, end;
	FILE *file = fopen("graph1.txt","w");
	
	fprintf (file,"\t\tMain\tOpenMP\tPthread\n");
	
	for (j=0;j<QTD_MAX; j++){
		N = qtdElementos[j];
		num_threads = qtdThreads[j];
		soma=0;	
		
		inter = N/num_threads;
		
		for (i=0;i<NUM_EXEC; i++){
			
			//inicializando as variáveis
			begin = clock();
			imprimePrimosSemThreads();
			end = clock();
			tempo = (double)(end-begin)/CLOCKS_PER_SEC;
			soma+=tempo;
			printf ("Execucao %2d com %ld numeros em %lf s\n",i+1,N,tempo);
		
		}
		fprintf (file,"%d\t%lf\t",N,soma/NUM_EXEC);
		
		soma=0;
		
		printf ("Executando N = %d com %d Threads\n", N, getNumThreads());
		for (i=0;i<NUM_EXEC;i++){
			//inicializando as variáveis
			inicio=0; fim=inter; count=0;
			
			//inicializando contador de tempo
			begin = clock();
			OpenMP();
			end = clock();
			tempo = (double)(end-begin)/CLOCKS_PER_SEC;
			soma=soma+tempo;

			printf ("Execucao %2d openmp em %lf s\n",i+1,tempo);
		}
		
		printf ("Media de 10 execucoes com openmp em %lf s\n\n",soma/NUM_EXEC);
	    fprintf (file,"%lf\t",soma/NUM_EXEC);
		soma=0;
	
		for (i=0;i<NUM_EXEC;i++){
			//inicializando as variáveis
			inicio=0; fim=inter; count=0;
			
			//inicializando contador de tempo
			begin = clock();
			Threads();
			end = clock();
			tempo = (double)(end-begin)/CLOCKS_PER_SEC;
			soma+=tempo;
			printf ("Execucao %2d thread em %lf s\n",i+1,tempo);
		}
		fprintf (file,"%lf\n",soma/NUM_EXEC);
		printf ("Media de 10 execucoes com threads em %lf s\n",soma/NUM_EXEC);
		
		
	}
}
