
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char *argv[]){
	double inicio, fin;
	int m, n, p, q, i, j, k;
	int *first, *second, *multiply;
	int rank, size;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(rank == 0){	// El coordinador
		srand(time(NULL));
		printf("Enter the number of rows and columns of first matrix: \n");
		scanf("%d%d", &m, &n);
		first = malloc(m*n*sizeof(int));
		for (i = 0; i < m; i++)
			for (j = 0; j < n; j++)
				first[i*n+j] = 1 + rand() % 10;

		printf("Enter the number of rows and columns of second matrix:\n");
		scanf("%d%d", &p, &q);
		second = malloc(m*n*sizeof(int));
		if(n != p){
			printf("Matrix with entered orders can't be multiplied with each other.\n");
			MPI_Abort(MPI_COMM_WORLD, -1);
			exit(-1);
		}
		printf("Enter the elements of second matrix\n");
		for (i = 0; i < p; i++)
			for (j = 0; j < q; j++)
				second[i*q+j] = 1 + rand() % 10;

		printf("\nMatriz A");
		for(i=0;i<m;i++){
			printf("\n");
			for(j=0;j<n;j++){
				printf("%d ", first[i*n +j]);
			}
		}
		printf("\n\nMatriz B");
		for(i=0;i<p;i++){
			printf("\n");
			for(j=0;j<q;j++){
				printf("%d ", second[i*q+j]);
			}
		}
		printf("\n\n");
	}
	inicio = MPI_Wtime();

	//Enviamos la primera matriz
	MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&p, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&q, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&first, m*n, MPI_INT, 0, MPI_COMM_WORLD);

	//Enviamos la segunda matriz
	for(i=0; i < p; i++){
		for(j=0; j < q; j++){
			if(j % size != 0){
				if(rank == 0){
					MPI_Send(&second[i*q+j], 1, MPI_INT, j % size, 1, MPI_COMM_WORLD);
				}else if(rank == j % size){
					MPI_Recv(&second[i*q+j], 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
				}
			}
		}
	}

	// Cálculo
	for (i = 0; i < m; i++) {
		for (j = 0; j < q; j++) {
			if(j % size == rank){	// cada proceso su columna
				multiply[i*n+j]=0;
				for(k = 0; k < p; k++){
					multiply[i*n+j]=(multiply[i*n+j]+(first[i*n+k]*second[k*q+j]));
				}
			}
		}
	}

	// Recibimos el cálculo de los esclavos en el coordinador
	for(i = 0; i < m; i++){
		for(j = 0; j < q; j++){
			if(j % size != 0){
				if(rank == j % size){
					MPI_Send(&multiply[i*n+j], 1, MPI_INTEGER, 0, 1, MPI_COMM_WORLD);
				}
				if(rank == 0) {
					MPI_Recv(&multiply[i*n+j], 1, MPI_INTEGER, j % size, 1, MPI_COMM_WORLD, &status);
				}
			}
		}
	}

	// Coordinador saca por pantalla
	fin = MPI_Wtime();
	if(rank == 0){
		printf("Product of entered matrix:\n ");
		for (i = 0; i < m; i++) {
			for (j = 0; j < q; j++)
				printf("%d\t", multiply[i*n+j]);
			printf("\n ");
		}
		printf("\nTiempo: %f\n", fin - inicio);
		FILE *fd;
		fd = fopen("program1.txt","a");
		fprintf(fd, "%d\t%f\n", size, fin - inicio);
		fclose(fd);
	}

	MPI_Finalize();
	return 0;
}
