#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>

int main(int argc, char *argv[]){
	int myrank , size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	printf("Hola soy el proceso %d de %d\n", myrank, size);
	MPI_Finalize();
	exit(0);
}
