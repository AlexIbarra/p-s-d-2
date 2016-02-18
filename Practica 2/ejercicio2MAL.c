#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main(int argc, char *argv[]){ 


	int numProcesos,rank,errcode,numWorkers;
	int x=0,i,j,k,dest,offset,source;
	int N=2;
	int vectorA[N][N],vectorB[N][N],vectorC[N][N];
	MPI_Status status;


	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank); //Devuelve el ID (rank) del proceso asociado a un comunicador
	MPI_Comm_size(MPI_COMM_WORLD, & numProcesos); //Devuelve el número de procesos relacionados con el comunicador

	
	double inicio,fin;

	inicio=MPI_Wtime();



	if(numProcesos < N+1){
		printf("Necesitas al menos %d MPI tasks. Saliendo....\n",N);	
		MPI_Abort(MPI_COMM_WORLD,errcode);
		exit(1);
	}

	numWorkers=numProcesos-1;	
	
	//***************************** PARTE DEL MAESTRO ************************************
	if(rank == 0){ 
		
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
		
		//MUESTRO LAS MATRICES INICIALES
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

		printf("Hasta aqui 1\n");

		//int MPI_Send( void* buf,int count, MPI_Datatype datatype, int destination,int tag,MPI_Comm comm );

		//int MPI_Bcast( void* buffer,int count,MPI_Datatype datatype, int root,MPI_Comm comm);


		for(dest = 1; dest <= numWorkers; dest++){
			for(i = 0; i < N; i++){
				MPI_Send(&vectorB[i][dest],1,MPI_INT,dest,0,MPI_COMM_WORLD);	//Columna correspondiente de la Matriz B
			}

		}
		printf("Hasta aqui 3\n");

		MPI_Bcast( vectorA[0] ,N*N ,MPI_INT, 0 ,MPI_COMM_WORLD);	//Matriz A
		
		printf("Hasta aqui 8\n");

		for(i=1;i<=numWorkers;i++){
			source = i;
			for(j = 0; j < N; j++){
				MPI_Recv(&vectorC[j][i],1,MPI_INT,i,0,MPI_COMM_WORLD,&status);			
			}

		
		}

		
		printf("Hasta aqui 9\n");

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
	}
	//***************************** PARTE DE LOS WORKERS ************************************

	if( rank > 0){  //Si es un worker
		
		printf("Hasta aqui 2\n");	

		for(i = 0; i < N; i++){
			MPI_Recv(&vectorB[i][rank],1,MPI_INT,0,0,MPI_COMM_WORLD,&status);	//Columna correspondiente de la Matriz B
		}
		
		MPI_Bcast( vectorA , N*N,MPI_INT, 0 ,MPI_COMM_WORLD);	//Matriz A

		printf("Hasta aqui 4\n");

		for(j = 0; j < N ; j ++){
			for(k=0;k<N;k++){
			      *vectorC[j][rank - 1]=(*vectorC[j][rank - 1]+(*vectorA[j][k]* *vectorB[k][rank-1]));
			}
		}
	
		printf("Hasta aqui 5\n");
		
		for(i=0;i<N;i++){	  
		  	printf("  %d  ", vectorC[i][rank]);
			printf("\n");
	  	}

		for(j = 0; j < N; j++){
			MPI_Send(&vectorC[i][rank],1,MPI_INT,0,rank,MPI_COMM_WORLD);	//Columna correspondiente de la Matriz B
		}
		
		printf("Hasta aqui 7\n");
	}

	
	fin=MPI_Wtime();

	//printf("El tiempo de ejecucion del programa es: %d \n",fin-inicio);
	MPI_Finalize();
	exit(0);
}
