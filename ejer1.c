#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv)
{
  int i, j, m, n, p, q, c, d, k, sum = 0, rank, size;
  //int first[10][10], second[10][10], multiply[10][10];
  int * first, * second, * multiply;

  
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Status status;

  srand(time(NULL));

  double fin, inicio;


  if(rank == 0) {

    printf("Enter the number of rows and columns of first matrix\n");
    scanf("%d%d", &m, &n);
   //printf("Enter the elements of first matrix\n");
  }

  MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);//Envio de la dimension
  MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);//Envio de la dimension

  //printf("Soy %d con m=%d y n=%d\n",rank, m, n);

  first = (int *)malloc(m*n*sizeof(int));

  if(rank == 0) {
   
    for (c = 0; c < m; c++)
      for (d = 0; d < n; d++)
        first[c*n+d] = rand() % 11;
   
    printf("Enter the number of rows and columns of second matrix\n");
    scanf("%d%d", &p, &q);

  }

  MPI_Bcast(&p, 1, MPI_INT, 0, MPI_COMM_WORLD);//Envio de la dimension
  MPI_Bcast(&q, 1, MPI_INT, 0, MPI_COMM_WORLD);//Envio de la dimension

  //printf("Soy %d con p=%d y q=%d\n",rank, p, q);

  second = (int *)malloc(p*q*sizeof(int));

  if(rank == 0) {
   
    if (n != p)
      printf("Matrices with entered orders can't be multiplied with each other.\n");
    else
    {
   // printf("Enter the elements of second matrix\n");
   
      for (c = 0; c < p; c++)
        for (d = 0; d < q; d++)
          second[c*q+d] = rand() % 11;
    }

  }

  multiply = (int *)malloc(m*q*sizeof(int));

  inicio = MPI_Wtime();
  
  MPI_Bcast(&first, m*n, MPI_INT, 0, MPI_COMM_WORLD); //Envio y recepcion de la matriz A



  //Envio de la matriz B. Cada columna a un proceso.
  for(i = 0; i < p; i++) {
    for(j = 0; j < q; j++) { 
      if(j%size != 0) { // Para evitar que el P0 se lo envie a si mismo

        if(rank == 0) {
          MPI_Send(&second[i*q+j], 1, MPI_INT, j%size, 1, MPI_COMM_WORLD);
        } 
        else if(rank == j%size) {
          MPI_Recv(&second[i*q+j], 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        }
      }
    }
  }


  /*for (c = 0; c < p; c++) {
      for (d = 0; d < q; d++)
        printf("%d\t", second[c*q+d]);

      printf("\n");
  }*/



  //Opera cada proceso su columna correspondiente
  for (c = 0; c < m; c++) {
    for (d = 0; d < q; d++) {
      if(d%size == rank){ // cada proceso su columna
        for (k = 0; k < p; k++) {
          sum = sum + first[c*n+k]*second[k*q+d];
        }
        multiply[c*n+d] = sum;
        sum = 0;
      }      
    }
  }

  //printf("HOLA\n");

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

    free(first);
    free(second);
    free(multiply);
  }
 
  MPI_Finalize();
  
 
  return 0;
}

