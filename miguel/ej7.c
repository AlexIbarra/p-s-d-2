#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv){

double fin, inicio;

	inicio = MPI_Wtime();

  	srand(time(NULL));
	int R, i, rank, size;
	int n = 9;

	int V[n], U[n];


	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int Va[n/size], Ua[n/size];

	if(rank == 0){
		if(!((n)%size) == 0 ){
			printf("La carga de trabajo quedaria descompensada con un vector de estas dimensiones.\n");
			MPI_Abort(MPI_COMM_WORLD, 0);
	 		MPI_Finalize();
			exit(0);
		}else{
			for(i = 0; i < n; i++){
				V[i] = rand() % 11; //Rellena los vectores aleatoriamente con numero entre 0 y 10
			}

			for(i = 0; i < n; i++){
				U[i] = rand() % 11; //Rellena los vectores aleatoriamente con numero entre 0 y 10
			}
		}
	}
	//Envio de los elemnetos del vector V equitativamente.
	MPI_Scatter(V, n/size, MPI_INT, Va, n/size, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);

	//Envio de los elemnetos del vector U equitativamente.
	MPI_Scatter(U, n/size, MPI_INT, Ua, n/size, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);

	//Operacion
	int Ra = 0;
	for(i = 0; i < n/size; i++){
		Ra += Va[i]*Ua[i];
	}

	//Recepción de los datos
	MPI_Reduce(&Ra, &R, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	//Impresion de los vectores y el resultado
	if(rank == 0){
		printf("V: ");
		for(i = 0; i < n; i++){ 
			printf("%d, ", V[i]);
		}

		printf("\nU: ");
		for(i = 0; i < n; i++){
			printf("%d, ", U[i]);
		}

		printf("\nResultado: %d\n", R);
	fin = MPI_Wtime();
	printf("tiempo: %f\n", fin-inicio);
	}

 	MPI_Finalize();
	return(0);
}
