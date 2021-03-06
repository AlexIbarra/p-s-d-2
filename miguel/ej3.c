#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv){

  srand(time(NULL));
  int i, j, k, rank, size;
  int f = 3;


double fin, inicio;

	inicio = MPI_Wtime();

  int A[f][f], B[f][f], C[f][f];
  MPI_Status status;
  MPI_Request req;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);


  MPI_Status status_list[6];
  MPI_Request request[6];

  // Master process?
  	if (rank == 0){
	  /*printf("Introduce la dimension de las matrices\n");
	  scanf("%d", &f);*/

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

	//Envio de la matriz B. Cada columna a un proceso.
	int r = 0;
	for(i = 0; i < f; i++){
		for(j = 0; j < f; j++){
	
			if (rank== 0){
				if(i%size != 0){
					MPI_Isend(&B[j][i], 1, MPI_INTEGER, i%size, 1, MPI_COMM_WORLD, &request[r]);
					r++;
				}
			}else if(rank == i%size) {
				MPI_Recv(&B[j][i], 1, MPI_INTEGER, 0, 1, MPI_COMM_WORLD, &status);
				//printf( "Recibido en proceso %d -> %d\n", rank, B[j][i] );
			}
		}
	}

	if(rank == 0)
		MPI_Waitall(6, request, status_list);

	//Opera cada proceso su columna correspondiente
	for(i=0;i<f;i++){
		for(j=0;j<f;j++){
			if(j%size == rank){
				C[i][j]=0;
				for(k=0;k<f;k++){
					C[i][j]=(C[i][j]+(A[i][k]*B[k][j]));
				}
			}
		}
		
	}

	//Los esclavos envian los resultados al maestro
	r = 0;
	for(i = 0; i < f; i++){
		for(j = 0; j < f; j++){
			if(j%size == rank){
				MPI_Send(&C[i][j], 1, MPI_INTEGER, 0, 1, MPI_COMM_WORLD);
			}else if(rank == 0) {
				MPI_Irecv(&C[i][j], 1, MPI_INTEGER, j%size, 1, MPI_COMM_WORLD, &request[r]);
				r++;
			}
		}
	}

	if(rank == 0)
		MPI_Waitall(6, request, status_list);

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
  return(0);


}
