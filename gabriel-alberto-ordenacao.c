#include <stdio.h>
#include <omp.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>
#define QTD_MAX 6   //bora testar com esses
#define NUM_THREADS getNumThreads()
#define INTERVALO getIntervalo()

pthread_mutex_t mutexsum;
int inicio=0, fim=999, count=0;
long int TAM, num_threads, inter;
long int qtdElementos[QTD_MAX] = {1000, 20000, 50000, 100000,	500000, 1000000};
int   qtdThreads[QTD_MAX] = {2, 4, 8, 16, 32, 64};
FILE *mainF;

int getNumThreads(){
	return num_threads;
}

void *thread_fun(int *vetor1, int *vetor2, int inicio, int fim);
void preencheVetor(int *vetor, int inicio, int fim);
void imprimeVetor(int *vetor,int inicio, int fim);
void copiaVetor(int *vet1, int *vet2, int inicio, int fim);
void quickSort(int *vet, int esq, int dir);
int particao (int vet[], int esq, int dir); //do quick
void troca(int* a, int* b);//do quick

int main(){
	mainF = fopen("graph.txt","w+");
	long int i, j;
	double tempo, soma=0,aux;
	clock_t begin, end;
	
	fprintf (mainF, "\t\tMain\tOpenMP\tPThread\n");
	for (j=0;j<QTD_MAX; j++){
		TAM = qtdElementos[j];
		num_threads = qtdThreads[j];
		fprintf (mainF,"%ld\t",TAM);
		inter = TAM/num_threads;
		
		//execucao na main
		
		
		int *vet  = (int*) malloc(TAM*sizeof(int));
		int *vet2 = (int*) malloc(TAM*sizeof(int));
		int *vet3 = (int*) malloc(TAM*sizeof(int));
		int *vet4 = (int*) malloc(TAM*sizeof(int));
		int *vet5 = (int*) malloc(TAM*sizeof(int));
		int *vet6 = (int*) malloc(TAM*sizeof(int));
		
		inicio=0; fim=TAM;
		/*
		//contabilizando tempo da main
		begin = clock();
		preencheVetor(vet, inicio, fim);
		copiaVetor(vet,vet2, inicio, fim);
		quickSort(vet2,inicio, fim);
		imprimeVetor(vet2, inicio, fim);
		end = clock();
		
		tempo = (double)(end - begin);
		fprintf (mainF,"%lf\t",tempo / CLOCKS_PER_SEC);
		tempo = 0;
		*/
		//execucao com open
		int *inicios = (int*) malloc(num_threads*sizeof(int));
		int *finais = (int*) malloc(num_threads*sizeof(int));
		inicio=0; fim=inter;
		printf ("1-%d-%d\n",TAM,inter);
		/*
		for (i=0;i<num_threads;i++){
			inicios[i] = inicio;
			finais[i] = fim;
			inicio = fim+1;
			fim+=inter;
			printf ("[%d,%d]\n", inicios[i], finais[i]);
		}
		*/
		printf ("\n\n");
		/*
		printf ("executando com open\n");
		
		//contabilizando tempo da main
		begin = clock();
		//setting threads max
		omp_set_num_threads(num_threads);
		
	    #pragma omp parallel
	    {
	    	#pragma omp for ordered
			for (i=0;i<num_threads;i++){
				#pragma omp ordered
				preencheVetor(vet3, inicios[omp_get_thread_num()], finais[omp_get_thread_num()]);
				copiaVetor(vet3,vet4, inicios[omp_get_thread_num()], finais[omp_get_thread_num()]);
				quickSort(vet4,inicios[omp_get_thread_num()], finais[omp_get_thread_num()]);
				imprimeVetor(vet4,inicios[omp_get_thread_num()], finais[omp_get_thread_num()]);
			}
			
		}
		
		end = clock();
		tempo = (double)(end - begin);
		fprintf (mainF,"%lf\t",(tempo / CLOCKS_PER_SEC));
		
		
		//execucao com thread
		begin = clock();
		pthread_mutex_init(&mutexsum, NULL);
		pthread_t *threads = (pthread_t*) malloc(num_threads*sizeof(pthread_t));
		
		for (i=0;i<num_threads;i++){
			int status = pthread_create(&(threads[i]), NULL, thread_fun(vet5,vet6,inicios[i],finais[i]), NULL);
			
		}
		
		for (i=0;i<num_threads;i++){
			pthread_join(threads[i],NULL);
		}
		
		pthread_mutex_destroy(&mutexsum);
		end = clock();
		tempo = (double)(end - begin);
		fprintf (mainF,"%lf\n",(tempo / CLOCKS_PER_SEC));
		free(threads);
		*/
		free(vet);
		free(vet2);
		free(vet3);
		free(vet4);
		free(vet5);
		free(vet6);
		free(inicios);
		free(finais);
		
		
	}
	
}

void preencheVetor(int *vetor, int inicio, int fim){
	printf ("preenchendo\n\n");
	int i;
	for (i=inicio; i<=fim; i++){
		vetor[i] = i+1;
	}

    int escolha = 0,rand_num = fim,pos_vetor = inicio,tmp;
    while( pos_vetor < fim )
    {
        /** Escolhe uma posicao aleatoria **/
        escolha = rand() % rand_num;
        /** Faz a troca pela posicao sequencial **/
        tmp = vetor[pos_vetor];
        vetor[pos_vetor] = vetor[escolha];
        vetor[escolha] = tmp;
        /** Proxima posicao que sera trocada **/
        pos_vetor++;
    }
    
}

void imprimeVetor(int *vetor,int inicio, int fim){
	int i;
	for (i=inicio;i<=fim;i++){
		printf ("%d\t",vetor[i]);
	}
}

void copiaVetor(int *vet1, int *vet2, int inicio, int fim){
	int i;
	for (i=inicio;i<=fim;i++){
		vet2[i]=vet1[i];
	}
}


// A utility function to troca two elements 
void troca(int* a, int* b) 
{ 
    int t = *a; 
    *a = *b; 
    *b = t; 
} 
  
/* This function takes last element as pivot, places 
the pivot element at its correct position in sorted 
vetay, and places all smaller (smaller than pivot) 
to left of pivot and all greater elements to right 
of pivot */
int particao (int vet[], int esq, int dir) 
{ 
    int pivot = vet[dir]; // pivot 
    int j, i = (esq - 1); // Index of smaller element and indicates the right position of pivot found so far
  
    for (j = esq; j <= dir - 1; j++) 
    { 
        // If current element is smaller than the pivot 
        if (vet[j] < pivot) 
        { 
            i++; // increment index of smaller element 
            troca(&vet[i], &vet[j]); 
        } 
    } 
    troca(&vet[i + 1], &vet[dir]); 
    return (i + 1); 
} 
  
/* The main function that implements QuickSort 
vet[] --> vetay to be sorted, 
esq --> Starting index, 
dir --> Ending index */
void quickSort(int *vet, int esq, int dir) 
{ 
    if (esq < dir) 
    { 
        /* pi is particaoing index, vet[p] is now 
        at right place */
        int pi = particao(vet, esq, dir); 
  
        // Separately sort elements before 
        // particao and after particao 
        quickSort(vet, esq, pi - 1); 
        quickSort(vet, pi + 1, dir); 
    } 
} 

  
void *thread_fun(int *vetor1, int *vetor2, int inicio, int fim){
	pthread_mutex_lock (&mutexsum);
	
	preencheVetor(vetor1, inicio, fim);
	copiaVetor(vetor1,vetor2, inicio, fim);
	quickSort(vetor2,inicio, fim);
	imprimeVetor(vetor2,inicio, fim);
	
	pthread_mutex_unlock (&mutexsum);
//	pthread_exit(NULL);
}
