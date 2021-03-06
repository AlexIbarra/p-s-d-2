#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv){

  double fin, inicio;

  inicio = MPI_Wtime();

  srand(time(NULL));
  int i, j, k, rank, size;
  int f = 3;

  int A[f][f], B[f][f], C[f][f];

  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);


  int Ba[f/size][f], Ca[f/size][f];

  // Master process?
  if (rank == 0){
	  printf("Introduce la dimension de las matrices\n");
	  scanf("%d", &f);

	if(!((f)%size) == 0 ){
		printf("La carga de trabajo quedaria descompensada con una matriz de estas dimensiones.\n");
		MPI_Abort(MPI_COMM_WORLD, 0);
 		MPI_Finalize();
		exit(0);
	}else{

	  for(i=0;i<f;i++){
	      for(j=0;j<f;j++){
		  A[i][j] = rand() % 11; //asigna un random del 0 al 10
	      }
	  }

	  for(i=0;i<f;i++){
	      for(j=0;j<f;j++){
		  B[i][j] = rand() % 11; //asigna un random del 0 al 10
	      }
	  }
	}
  }


	MPI_Bcast(&f, 1, MPI_INT, 0, MPI_COMM_WORLD);//Envio de la dimension

	MPI_Bcast(A, f*f, MPI_INT, 0, MPI_COMM_WORLD); //Envio y recepcion de la matriz A


	//transmutacion de la matriz
	int Bt[f][f];
	if(rank == 0){
		for(i=0;i<f;i++){
			for(j=0;j<f;j++){
			  Bt[i][j] = B[j][i];
			}
		}
	}



	//Envio de la matriz B. Cada columna a un proceso.
	MPI_Scatter(Bt, (f*f)/size, MPI_INT, Ba, (f*f)/size, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);

	/*printf("\n Matriz B del proceso %d -> %d\n", rank, Ba[0][0]);
	printf("\n Matriz B del proceso %d -> %d\n", rank, Ba[0][1]);
	printf("\n Matriz B del proceso %d -> %d\n", rank, Ba[0][2]);*/

	//Opera cada proceso su columna correspondiente
	for(i=0;i<f;i++){
		for(j=0;j<f/size;j++){
			Ca[j][i]=0;
			for(k=0;k<f;k++){
				Ca[j][i]=(Ca[j][i]+(A[i][k]*Ba[j][k]));
			}
			//printf("\nproceso C[%d][%d]%d -> %d",j ,i, rank, Ca[j][i]);
			
		}
		
	}

	int Ct[f][f];
	MPI_Gather(Ca, (f*f)/size, MPI_INT, Ct, (f*f)/size, MPI_INT, 0, MPI_COMM_WORLD);
	
	MPI_Barrier(MPI_COMM_WORLD);

	//transmutacion de la matriz
	if(rank == 0){
		for(i=0;i<f;i++){
			for(j=0;j<f;j++){
			  C[i][j] = Ct[j][i];
			}
		}
	}


	if(rank == 0){
	
		  printf("\n\n\t\t\t Matriz A");
		  printf("\n");
		  for(i=0;i<f;i++){
		      printf("\n\t\t");
		      for(j=0;j<f;j++){
			  printf("  %6d  ", A[i][j]);
		      }
		  }
		  printf("\n\n\t\t\t Matriz B");
		  printf("\n");
		  for(i=0;i<f;i++){
		      printf("\n\t\t");
		      for(j=0;j<f;j++){
			  printf("  %6d  ", B[i][j]);
		      }
		  }
		 
		  printf("\n\n\t\t\t Matriz C");
		  printf("\n");
		  for(i=0;i<f;i++){
		      printf("\n\t\t");
		      for(j=0;j<f;j++){
			  printf("  %6d  ", C[i][j]);
		      }
		  }
		  printf("\n");
	fin = MPI_Wtime();
	printf("tiempo: %f\n", fin-inicio);
	}

  MPI_Finalize();
  exit(0);


}
