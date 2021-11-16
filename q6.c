#include <stdio.h>
#include <omp.h>

void inverteLinhaMatriz(int matriz[3][3], int tid){
	int i, aux;
	aux = matriz[tid][0];
	matriz[tid][0] = matriz[tid][2];
	matriz[tid][2] = aux;
		
}

void imprime(int M[3][3]){
	int i,j;
	for (i=0;i<3;i++){
		for (j=0;j<3;j++)
			printf ("%3d",M[i][j]);
		printf ("\n");
	}
}

int main(){
	
	int M[3][3]={1,2,3,4,5,6,7,8,9}, somas[3];
	int i,j;
	imprime(M);
	printf ("\n\n");
	omp_set_num_threads(3);
	
	#pragma omp parallel
	{
		#pragma omp for
		for (i=0;i<omp_get_num_threads();i++)
			inverteLinhaMatriz(M,omp_get_thread_num());
	}
	
	imprime(M);
	
	return 0;
}
