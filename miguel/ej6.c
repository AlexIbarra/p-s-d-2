#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv){


	clock_t start, end;
	double cpu_time_used;
	start = clock();

	int n, i;

	printf("Introduce el numero de elementos del vector.\n");
  	scanf("%d", &n);

	int V[n], U[n];
	for(i = 0; i < n; i++){
		V[i] = rand() % 11; //Rellena los vectores aleatoriamente con numero entre 0 y 10
	}

	for(i = 0; i < n; i++){
		U[i] = rand() % 11; //Rellena los vectores aleatoriamente con numero entre 0 y 10
	}

	//Operacion
	int r = 0;
	for(i = 0; i < n; i++){
		r += V[i]*U[i];
	}

	//Impresion de los vectores y el resultado
	printf("V: ");
	for(i = 0; i < n; i++){
		printf("%d, ", V[i]);
	}

	printf("\nU: ");
	for(i = 0; i < n; i++){
		printf("%d, ", U[i]);
	}
	printf("\nResultado: %d\n", r);

	/*end = clock();
	cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("Tiempo invertido = %lf \n",cpu_time_used);*/

}
