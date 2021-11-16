#include <stdio.h>
#include <omp.h>

int somaLinhaMatriz(int matriz[3][3], int tid){
	int i, soma=0;
	for (i=0;i<3;i++)
		soma+=matriz[tid][i];
	
	return soma;
}

int main(){
	
	int M[3][3]={1,2,3,4,5,6,7,8,9}, somas[3];
	int i,j;

	omp_set_num_threads(3);
	#pragma omp parallel
	{
		#pragma omp for
		for (i=0;i<omp_get_num_threads();i++)
			somas[omp_get_thread_num()] = somaLinhaMatriz(M,omp_get_thread_num());
	}
	
	for (i=0;i<3;i++){
		printf ("Soma linha[%d]: %d\n", i+1,somas[i]);
	}
	
	return 0;
}
