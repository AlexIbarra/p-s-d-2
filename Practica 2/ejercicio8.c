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
  int numElements, offset, stripSize, myrank, numnodes, N,i,j;
  
  MPI_Init(&argc, &argv);
  
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  MPI_Comm_size(MPI_COMM_WORLD, &numnodes);
  
  N = atoi(argv[1]);

  int resto;
  numElements = N/numnodes;

  int vectorA[N];
  int vectorB[N];
  int vectorC[numElements];

  int tamanio[numnodes]; //Array que guarda el tamaño de los datos para cada worker
  int desplazamiento[numnodes]; //Array que guarda el desplazamiento necesario

  int result = 0;
 
  int auxA[numElements];
  int auxB[numElements];
  int auxC[numElements];
  int auxRes = 0;

  resto = N % numnodes;

//Si es el coordinador inicializo los Vectores A y B e inicio el contador de tiempo.
  if (myrank == 0) {
    for (i=0; i<N; i++) {
      vectorA[i]=i;
      vectorB[i]=i+N;
      vectorC[i]=0;
    }
    startTime = MPI_Wtime();
  }

	//Rellenamos los vectores con los datos correspondientes segun la dimension y el numero de procesos lanzados
    
    desplazamiento[0] = 0;

    for(j=0;j < (numnodes - resto) ; j++){

	tamanio[j] = numElements;
	desplazamiento[j+1] = desplazamiento[j] + numElements;

    }

    for( j = numnodes - resto ; j < numnodes; j++){
     	tamanio[j] = numElements + 1;
        if( j != numnodes - 1){
            desplazamiento[j+1] = desplazamiento[j] + numElements + 1;
	}
    }


    MPI_Scatterv(&vectorA,tamanio,desplazamiento,MPI_INT, &auxA, tamanio[myrank], MPI_INT, 0,MPI_COMM_WORLD);
    MPI_Scatterv(&vectorB,tamanio,desplazamiento,MPI_INT, &auxB, tamanio[myrank], MPI_INT, 0,MPI_COMM_WORLD);

	//Realizan operaciones los workers
   
   for(i = 0; i<N; i++){
	auxC[i] = auxA[i] * auxB[i];
	auxRes = auxRes + auxC[i];
  
   }
   
   MPI_Reduce(&auxRes,&vectorC,numElements,MPI_INT,MPI_SUM,0, MPI_COMM_WORLD); //Sumamos los resultados parciales en un resultado final

	//Imprimimos resultados por pantalla

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
		result += vectorC[i]; //Resultado del producto escalar
	}

	printf("%d\n",result);

	printf("\n\n");
  
	endTime = MPI_Wtime();

	printf("Ha tardado %f segundos.\n\n", endTime-startTime);


   }
	MPI_Finalize();
	
	exit(0);

     


}  
