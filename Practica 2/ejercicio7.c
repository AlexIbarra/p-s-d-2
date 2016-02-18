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
  int numElements, offset, stripSize, myrank, numnodes, N,i;
  
  MPI_Init(&argc, &argv);
  
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &numnodes);
  
  N = atoi(argv[1]);
   
  numElements = N/numnodes; //Tamaño de los datos para cada worker

  int vectorA[N];
  int vectorB[N];
  int vectorC[numElements];

  int result = 0;
 
  int auxA[numElements];
  int auxB[numElements];
  int auxC[numElements];


   if(myrank == 0 && N%numnodes != 0){ //Comprobamos que los datos sean divisibles por el numero de procesos
		printf("El tamaño de los datos tienen que ser divisibles por el numero de procesos.\nEl programa ha terminado...\n");
	}

   else{

	//Si es el coordinador inicializo los Vectores A y B e inicio el contador de tiempo.
	  if (myrank == 0) {
	    for (i=0; i<N; i++) {
	      vectorA[i]=i;
	      vectorB[i]=i+N;
	      vectorC[i]=0;
	    }
	    startTime = MPI_Wtime();
	  }

	    MPI_Scatter(&vectorA,numElements,MPI_INT, &auxA, numElements, MPI_INT, 0,MPI_COMM_WORLD);
	    MPI_Scatter(&vectorB,numElements,MPI_INT, &auxB, numElements, MPI_INT, 0,MPI_COMM_WORLD);

	//Realizamos operaciones
   
	   for(i = 0; i<N/numnodes; i++){
		auxC[i] = auxA[i] * auxB[i];
	   }
	
 	   MPI_Reduce(&auxC,&vectorC,numElements,MPI_INT,MPI_SUM,0, MPI_COMM_WORLD);

	//Mostramos resultados

	   if(myrank == 0){
		printf("Vector A: \n");
		for(i=0; i<N ; i++){
			printf("%d ",vectorA[i]);
		}

		printf("\n\n");
	
		printf("Vector B: \n");
		for(i=0; i<N ; i++){
			printf("%d ",vectorB[i]);
		}

		printf("\n\n");

		printf("Producto Escalar: \n");
		for(i=0; i<numElements ; i++){
			result += vectorC[i];
		}

		endTime = MPI_Wtime();

		printf("%d\n",result);//Mostramos el resultado final

		printf("\n\n");
	  
	   }	
		if(myrank==0){
			
			printf("Ha tardado %f segundos.\n\n", endTime-startTime);
		}

		

		MPI_Finalize();

		exit(0);

     }


}  
