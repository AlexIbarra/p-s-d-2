#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv)
{
  int i, j, m, n, p, q, c, d, k, sum = 0, rank, size;
  int first[100000], second[100000], multiply[100000];

  
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Status status;

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
        first[c*n+d] = 1 + rand() % 2;
   
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
          second[c*q+d] = 1 + rand() % 2;
    }

  }
  
  if(rank == 0)  {
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
  }

  inicio = MPI_Wtime();
  
  MPI_Bcast(&second, p*q, MPI_INT, 0, MPI_COMM_WORLD); //Envio y recepcion de la matriz B



  //Envio de la matriz B. Cada columna a un proceso.
  for(i = 0; i < m; i++) {
    for(j = 0; j < m; j++) { 
      if(j%size != 0) { // Para evitar que el P0 se lo envie a si mismo

        if(rank == 0) {
          MPI_Send(&first[i*n+j], 1, MPI_INT, j%size, 1, MPI_COMM_WORLD);
        } 
        else if(rank == j%size) {
          MPI_Recv(&first[i*n+j], 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        }
      }
    }
  }


  //Opera cada proceso su columna correspondiente
  for (c = 0; c < m; c++) {
    for (d = 0; d < q; d++) {
      if(d%size == rank){ // cada proceso su columna
		    multiply[c*n+d] = 0;
        for (k = 0; k < p; k++) {
          multiply[c*n+d] = (multiply[c*n+d] + (first[c*n+k]*second[k*q+d]));
        }
      }      
    }
  }

  //Envio de la matriz B. Cada columna a un proceso.
  for(i = 0; i < m; i++){
    for(j = 0; j < q; j++){
      if(j%size != 0) {
        if (rank == j%size){
          MPI_Send(&multiply[i*n+j], 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        }else if(rank == 0) {
          MPI_Recv(&multiply[i*n+j], 1, MPI_INT, j%size, 1, MPI_COMM_WORLD, &status);
        }
      }
    }
  }


  if(rank == 0) {

    printf("Product of entered matrices:\n");

    for (c = 0; c < m; c++) {
      for (d = 0; d < q; d++)
        printf("%d\t", multiply[c*n+d]);

      printf("\n");
    }
    fin = MPI_Wtime();
    printf("tiempo: %f\n", fin-inicio);
  }
 
  MPI_Finalize();
  
 
  return 0;
}


