#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv)
{
  int i, j, m, n, p, q, c, d, k, sum = 0, rank, size, cont;
  int first[100000], second[100000], multiply[100000], secondInv[100000];
  int tmpA[100000], tmpB[100000], tmpC[100000], tmpD[100000];

  
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Status status;
  MPI_Request request;
  MPI_Status status_list[6];
  MPI_Request request_list[6];

  srand(time(NULL));

  double fin, inicio;


  if(rank == 0) {

    printf("Enter the number of rows and columns of first matrix\n");
    scanf("%d%d", &m, &n);
  }

  MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);//Envio de la dimension
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);//Envio de la dimension


  if(rank == 0) {
   
    for (c = 0; c < m; c++)
      for (d = 0; d < n; d++)
        first[c*n+d] = rand() % 11;
   
    printf("Enter the number of rows and columns of second matrix\n");
    scanf("%d%d", &p, &q);

  }

  MPI_Bcast(&p, 1, MPI_INT, 0, MPI_COMM_WORLD);//Envio de la dimension
  MPI_Bcast(&q, 1, MPI_INT, 0, MPI_COMM_WORLD);//Envio de la dimension


  if(rank == 0) {
   
    if (n != p)
      printf("Matrices with entered orders can't be multiplied with each other.\n");
    else
    {
   
      for (c = 0; c < p; c++)
        for (d = 0; d < q; d++)
          second[c*q+d] = rand() % 11;
    }

  }

  inicio = MPI_Wtime();
  
  MPI_Bcast(&first, m*n, MPI_INT, 0, MPI_COMM_WORLD); //Envio y recepcion de la matriz A
  
  
  /*if(rank == 0)  {
	  // Invertimos la segunda matriz
		for (c = 0; c < p; c++)
			for (d = 0; d < q; d++)
				secondInv[c*p+d] = second[d*q+c];
				
		printf("\n");
		printf("\n");
		
		printf("Matriz A:\n");
		for (c = 0; c < m; c++) {
		  for (d = 0; d < n; d++)
			printf("%d\t", first[c*n+d]);
			
			printf("\n");
		}
			
		printf("\n");
		printf("\n");
		
		printf("Matriz B:\n");
		for (c = 0; c < p; c++) {
		  for (d = 0; d < q; d++)
			printf("%d\t", second[c*q+d]);
			
			printf("\n");
		}
			
		printf("\n");
		printf("\n");
	}*/
  
		
	
	
	// Enviamos Matriz B. Cada columna a un proceso.
	MPI_Scatter(&secondInv, (p*q)/size, MPI_INT, &tmpA, (p*q)/size, MPI_INT, 0, MPI_COMM_WORLD);


  MPI_Barrier(MPI_COMM_WORLD);
    



  //Opera cada proceso su columna correspondiente
  for (c = 0; c < m; c++) {
    for (d = 0; d < q; d++) {
      if(d%size == rank){ // cada proceso su columna
  		  printf("Rank %d calculando:\n", rank);
  		  tmpC[c*q+d] = 0;
        for (k = 0; k < p; k++) {
          //sum = sum + first[c*n+k]*tmpA[(d-rank)*p+k];
          tmpC[c*q+d] = (tmpC[c*q+d] + (first[c*n+k]*tmpA[(d-rank)*p+k]));
          printf("tmpC[%d] += ((%d*%d) = %d)\n", (c*q+d), first[c*n+k], tmpA[(d-rank)*p+k], tmpC[c*q+d]);
        }
        //tmpC[c*q+d] = sum;
        //sum = 0;
      }      
    }
  }


   //Envio de la matriz B. Cada columna a un proceso.
	MPI_Gather(&tmpC, (m*q)/size, MPI_INT, &multiply, (m*q)/size, MPI_INT, 0, MPI_COMM_WORLD);

  


  if(rank == 0) {

    printf("Product of entered matrices:\n");

    for (c = 0; c < m; c++) {
      for (d = 0; d < q; d++)
        printf("%d\t", multiply[c*q+d]);

      printf("\n");
    }
    fin = MPI_Wtime();
    printf("tiempo: %f\n", fin-inicio);
  }
 
  MPI_Finalize();
  
 
  return 0;
}

