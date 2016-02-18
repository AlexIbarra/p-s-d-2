//Realizado por:
//Ignacio Bartolomé Tabanera
//Roberto Morgado Luengo
//Rubén Soto Ponce

#include <stdio.h>
#include "mpi.h"


int main(int argc, char *argv[]){ 
	
	int myrank,size;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&myrank); //Devuelve el ID (rank) del proceso asociado a un comunicador
	MPI_Comm_size(MPI_COMM_WORLD, & size); //Devuelve el número de procesos relacionados con el comunicador

  double startTime, endTime;

	if (myrank == 0) {
    		startTime = MPI_Wtime();
  	}
	int x=0,i,j,k,N;
	N = atoi(argv[1]); //argumento pasado que indica el tamaño de las matrices cuadradas
	int vectorA[N][N],vectorB[N][N],vectorC[N][N];

	//Inicializaciones de matrices

	for(i=0;i < N;i++){
		for(j=0;j<N;j++){
			vectorA[i][j]=x;
			x++;
		
		}
	}

	x=0;
	for(i=0;i < N;i++){
		for(j=0;j<N;j++){
			vectorB[i][j]=x;
			x++;
		
		}
	}

	for(i=0;i < N;i++){
		for(j=0;j<N;j++){
			vectorC[i][j]=0;
		
		}
	}
	//Multiplicacion de las matrices

	for(i=0;i<N;i++){
	      for(j=0;j<N;j++){
		  for(k=0;k<N;k++){
		      vectorC[i][j]=(vectorC[i][j]+(vectorA[i][k]*vectorB[k][j]));
			
		  }
	      }
	  }

	//Muestra de las matrices

	if (myrank == 0) {
   		 endTime = MPI_Wtime();
  	}

	printf("Matriz A");
	  printf("\n");
	  for(i=0;i<N;i++){	  
	      for(j=0;j<N;j++){
		  printf("  %d  ", vectorA[i][j]);

	      }
		printf("\n");
	  }
  	printf("\n");
	printf("Matriz B");
	  printf("\n");
	  for(i=0;i<N;i++){	  
	      for(j=0;j<N;j++){
		  printf("  %d  ", vectorB[i][j]);

	      }
		printf("\n");
	  }
  	printf("\n");
	

	 printf("Matriz C");
	  printf("\n");
	  for(i=0;i<N;i++){	  
	      for(j=0;j<N;j++){
		  printf("  %d  ", vectorC[i][j]);

	      }
		printf("\n");
	  }
 	 printf("\n");
	
	
	  if (myrank == 0) {
	    printf("\n");
	    printf("Ha tardado %f segundos.\n\n", endTime-startTime);
	    printf("\n");
	  }

	
	MPI_Finalize();
	exit(0);
}
