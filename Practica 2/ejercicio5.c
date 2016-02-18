//Realizado por:
//Ignacio Bartolomé Tabanera
//Roberto Morgado Luengo
//Rubén Soto Ponce

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>


#define TAG 13

int main(int argc, char *argv[]) {

  double startTime, endTime;
  int numElements, offset, stripSize, myrank, numnodes, N, i, j, k,x;

  int tamanio[numnodes];
  int desplazamiento[numnodes];

  int resto;
  
  MPI_Init(&argc, &argv);
  
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &numnodes);

  N = atoi(argv[1]);

  resto = N % numnodes;
  
 
  double A[N][N], B[N][N], C[N][N]; 
  double auxA[N][N], auxC[N][N];

  if (myrank == 0) {
    // inicializar A y B
    x=0;
    for (i=0; i<N; i++) {
      for (j=0; j<N; j++) {
        A[i][j] = x;
        B[i][j] = x;
	      x++;
      }
    }
  }
  
  // Empezar el contador
  if (myrank == 0) {
    startTime = MPI_Wtime();
  }
  
  numElements = N/numnodes;  //Tamaño de los datos con los que opera cada trabajador.

  desplazamiento[0] = 0;

  for(j=0;j < (numnodes - resto) ; j++){

  	tamanio[j] = numElements * N;
  	desplazamiento[j+1] = desplazamiento[j] + (numElements * N);

  }

  for( j = numnodes - resto ; j < numnodes; j++){
 	  tamanio[j] = (N * (numElements + 1));
    if( j != numnodes - 1){
        desplazamiento[j+1] = desplazamiento[j] + (N * (numElements + 1));
    }
  }

  for(i = 0; i < numnodes ; i++){

  	printf("desplazamiento[%d] = %d \n",i,desplazamiento[i]);
  	printf("tamanio[%d] = %d \n",i,tamanio[i]);
  }


  //El master realiza un envio al resto de workers con la matriz A
  MPI_Scatterv(&A,tamanio,desplazamiento,MPI_DOUBLE, &auxA, tamanio[myrank], MPI_DOUBLE, 0,MPI_COMM_WORLD);
 
  // Todos obtienen B
  MPI_Bcast(&B, N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  // Cada proceso inicializa C a 0. 
  for (i=0; i<N; i++) {
    for (j=0; j<N; j++) {
      C[i][j] = 0.0;
    }
  }

  // Realizar operaciones
  for (i=0; i<N; i++) {
    for (j=0; j<N; j++) {
      for (k=0; k<N; k++) {
        auxC[i][j] += auxA[i][k] * B[k][j];
      }
    }
  }

  //El master junta los datos de la matriz con la solución
  MPI_Gatherv(&auxC,tamanio[myrank],MPI_DOUBLE,&C,tamanio,desplazamiento,MPI_DOUBLE,0,MPI_COMM_WORLD);

  // para el contador
  if (myrank == 0) {
    endTime = MPI_Wtime();
  }
  
   // Imprime la matriz A
  if (myrank == 0 && N < 10) {
    printf("Matriz A:\n");
    for (i=0; i<N; i++) {
      for (j=0; j<N; j++) {
        printf("%f ", A[i][j]);
      }
      printf("\n");
    }
  }
 // Imprime la matriz B 
  if (myrank == 0 && N < 10) {
    printf("\n");
    printf("Matriz B:\n");
    for (i=0; i<N; i++) {
      for (j=0; j<N; j++) {
        printf("%f ", B[i][j]);
      }
      printf("\n");
    }
  }


  // Imprime la matriz C 
  if (myrank == 0 && N < 10) {
    printf("\n");
    printf("Matriz C:\n");
    for (i=0; i<N; i++) {
      for (j=0; j<N; j++) {
        printf("%f ", C[i][j]);
      }
      printf("\n");
    }
  }

  if (myrank == 0) {
    printf("\n");
    printf("Ha tardado %f segundos.\n\n", endTime-startTime);
    printf("\n");
  }

  MPI_Finalize();
  return 0;
}


