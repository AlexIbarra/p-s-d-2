//Realizado por:
//Ignacio Bartolomé Tabanera
//Roberto Morgado Luengo
//Rubén Soto Ponce

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


int main(int argc, char *argv[]) {

	int N;

	N = atoi(argv[1]); //Dimension de los vectores

	int i,j,myrank, numnodes,result=0;

	int vectorA[N];
    int vectorB[N];
	int vectorC[N];

	double startTime, endTime;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &numnodes);


	startTime = MPI_Wtime();//Iniciamos el tiempo
	
	//Inicializamos matrices

	for(j=0 ; j<N ; j++){
		vectorA[j]=j;
		vectorB[j]=j+N;
	}

	//Realizamos operaciones

	for( i=0; i<N ;i++){
		vectorC[i]=vectorA[i] * vectorB[i];
		result= result + vectorC[i];
	}
	
	//Mostramos por pantalla	

	if(myrank==0){
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

		printf("Vector C: \n");
		for(i=0; i<N ; i++){
			printf("%d ",vectorC[i]);
		}

		printf("\nEl resultado del producto escalar es: %d",result); //Mostramos resultado

		printf("\n\n");

	
		endTime = MPI_Wtime(); //Finalizamos el tiempo
	
		printf("Ha tardado %f segundos.\n\n", endTime-startTime);

	}
        MPI_Finalize();

	exit(0);

}
