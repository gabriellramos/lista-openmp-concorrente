#include <stdio.h>
#include <omp.h>
#include <math.h>

#define N 99999
#define NUM_THREADS getNumThreads()

int inicio=0, fim=999, count=0;
void imprimePrimos(int tid);

int getNumThreads(){
	return (N / 1000)+1;
}


int main(int argc, char** argv){
    int thread_id, i;
	
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
    		fim+=1000;
		}
        
    }
    
    printf ("\n\nForam contados %d numeros primos no intervalo [0,%d]\n",count,N);

    return 0;
}


void imprimePrimos(int tid){
	int cont=0,i, k,j;
	int id = tid;
	
	k=0;
	printf ("\nTHREAD %3d - (%d a %d)\n",id, inicio, fim);
	
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
			count++;
			if (k==10){
				printf ("\n");
				k=0;
			}
		}
	}

}

