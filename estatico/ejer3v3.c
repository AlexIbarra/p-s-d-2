

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int first[600][600], second[600][600], multiply[600][600];

int main(int argc, char *argv[]){
	double inicio, fin;
	int m, n, p, q, i, j, k;

	int rank, size;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(rank == 0){	// El coordinador
		srand(time(NULL));
		printf("Enter the number of rows and columns of first matrix: \n");
		k=scanf("%d%d", &m, &n);
		printf("Enter the elements of first matrix\n");
		for (i = 0; i < m; i++)
			for (j = 0; j < n; j++)
				first[i][j] = 1 + rand() % 9;

		printf("Enter the number of rows and columns of second matrix:\n");
		k=scanf("%d%d", &p, &q);
		if(n != p){
			printf("Matrix with entered orders can't be multiplied with each other.\n");
			MPI_Abort(MPI_COMM_WORLD, -1);
			exit(-1);
		}
		if(q % size != 0){
			printf("Matrix with entered columns can't be properly reparted.\n");
			MPI_Abort(MPI_COMM_WORLD, -1);
			exit(-1);
		}
		printf("Enter the elements of second matrix\n");
		for (i = 0; i < p; i++)
			for (j = 0; j < q; j++)
				second[i][j] = 1 + rand() % 9;

		printf("\nMatriz A");
		for(i=0;i<m;i++){
			printf("\n");
			for(j=0;j<n;j++){
				printf("%d ", first[i][j]);
			}
		}
		printf("\n\nMatriz B");
		for(i=0;i<p;i++){
			printf("\n");
			for(j=0;j<q;j++){
				printf("%d ", second[i][j]);
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

	if(rank == 0)
		MPI_Scatter(&second[0][0], (p*q)/size, MPI_INT, MPI_IN_PLACE, (p*q)/size, MPI_INT, 0, MPI_COMM_WORLD);
	else
		MPI_Scatter(&second[0][0], (p*q)/size, MPI_INT, &second[0][0], (p*q)/size, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);

	// Cálculo

	for (i = 0; i < m; i++) {
		for (j = 0; j < q; j++) {
			multiply[i][j] = 0;
			for(k = 0; k < q; k++){
				multiply[i][j] += (first[i][k] * second[k][j]);
			}
		}
	}

	// Recibimos el cálculo de los esclavos en el coordinador
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank == 0)
		MPI_Gather(MPI_IN_PLACE, (m*q)/size, MPI_INT, &multiply[0][0], (m*q)/size, MPI_INT, 0, MPI_COMM_WORLD);
	else
		MPI_Gather(&multiply[0][0], (m*q)/size, MPI_INT, &multiply[0][0], (m*q)/size, MPI_INT, 0, MPI_COMM_WORLD);

	// Invertimos la matriz resultado
	/*if(rank == 0){
		for(i = 0; i < q; i++){
			for(j = 0; j < m; j++){
				multiply[j*m+i] = multiplyI[i*q+j];
			}
		}
	}*/

	// Coordinador saca por pantalla
	fin = MPI_Wtime();
	if(rank == 0){
		printf("Product of entered matrix:\n ");
		for (i = 0; i < m; i++) {
			for (j = 0; j < q; j++)
				printf("%d\t", multiply[i][j]);
			printf("\n ");
		}
		printf("\nTiempo: %f\n", fin - inicio);
		FILE *fd;
		fd = fopen("program3.txt","a");
		fprintf(fd, "%d\t%f\n", size, fin - inicio);
		fclose(fd);
	}



	MPI_Finalize();
	return 0;
}
