//Realizado por:
//Ignacio Bartolomé Tabanera
//Roberto Morgado Luengo
//Rubén Soto Ponce

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


#define TAG 13

int main(int argc, char *argv[]) {

  double startTime, endTime;
  int numElements, offset, stripSize, myrank, numnodes, N, i, j, k,x;
  MPI_Request request, request2;
  MPI_Status status;
  
  MPI_Init(&argc, &argv);
  
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &numnodes);


  N = atoi(argv[1]);
  
 
  double A[N][N], B[N][N], C[N][N]; 

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
  
  stripSize = N/numnodes;  //Tamaño de los datos con los que opera cada trabajador.

  // Enviar a cada worker la parte correspondiente de A
  if (myrank == 0) {
    offset = stripSize;    
    numElements = stripSize * N;
    for (i=1; i<numnodes; i++) {
      MPI_Isend(&A[offset], numElements, MPI_DOUBLE, i, i, MPI_COMM_WORLD, &request);
      MPI_Wait(&request,&status);
      offset += stripSize;
    }
     
  }
  else {  // Cada worker recibe su parte de A
    MPI_Recv(&A[0], stripSize * N, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  
  // Todos obtienen B
  MPI_Bcast(&B[0], N*N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  // Cada proceso inicializa C a 0. 
  for (i=0; i<N; i++) {
    for (j=0; j<N; j++) {
      C[i][j] = 0.0;
    }
  }

  // Realizan los workers las operaciones
  for (i=0; i<N; i++) {
    for (j=0; j<N; j++) {
      for (k=0; k<N; k++) {
	C[i][j] += A[i][k] * B[k][j];
      }
    }
  }

  // El maestro recibe los datos de los trabajadores.
  if (myrank == 0) {
    offset = stripSize; 
    numElements = stripSize * N;
    for (i=1; i<numnodes; i++) {
      MPI_Irecv(&C[offset], numElements, MPI_DOUBLE, i, i, MPI_COMM_WORLD, &request2);  /////
      offset += stripSize;
    }
  }
  else { // Cada worker envia su parte de C
    MPI_Send(&C[0], stripSize * N, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD);
  }

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
  exit(0);
}


