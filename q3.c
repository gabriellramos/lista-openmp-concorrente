#include <stdio.h>
#include <omp.h>

#define LIMITE 500

void crescente(int tid){
	int i;
	for (i=1; i<=LIMITE; i++)
		printf ("%d/%d\t",i,tid);
}

void decrescente(int tid){
	int i;
	for (i=LIMITE; i>0; i--)
		printf ("%d/%d\t",i,tid);
}

int main(){
	omp_set_num_threads(2);
	#pragma omp parallel
	{
		#pragma omp sections
		{
			#pragma omp section
			crescente(omp_get_thread_num());
			
			#pragma omp section
			decrescente(omp_get_thread_num());
		}	
	}
	return 0;
}
