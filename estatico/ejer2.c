#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv)
{
  int i, j, m, n, p, q, c, d, k, sum = 0, rank, size, cont;
  int first[100000], second[100000], multiply[100000];

  
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



  //Envio de la matriz B. Cada columna a un proceso.
  cont=0;
  for(i = 0; i < p; i++) {
    for(j = 0; j < q; j++) { 
      if(j%size != 0) { // Para evitar que el P0 se lo envie a si mismo

        if(rank == 0) {
          MPI_Isend(&second[i*q+j], 1, MPI_INT, j%size, 1, MPI_COMM_WORLD, &request);
          cont++;
        } 
        else if(rank == j%size) {
          MPI_Recv(&second[i*q+j], 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        }
      }
    }
  }


  MPI_Barrier(MPI_COMM_WORLD);



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

  //Envio de la matriz B. Cada columna a un proceso.
  cont=0;
  for(i = 0; i < m; i++){
    for(j = 0; j < q; j++){
      if(j%size != 0) {
        if (rank == j%size){
          MPI_Send(&multiply[i*n+j], 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        }else if(rank == 0) {
          MPI_Irecv(&multiply[i*n+j], 1, MPI_INT, j%size, 1, MPI_COMM_WORLD, &request);
          cont++;
        }
      }
    }
  }


  MPI_Barrier(MPI_COMM_WORLD);


  if(rank == 0) {

    //~ printf("Product of entered matrices:\n");
//~ 
    //~ for (c = 0; c < m; c++) {
      //~ for (d = 0; d < q; d++)
        //~ printf("%d\t", multiply[c*n+d]);
//~ 
      //~ printf("\n");
    //~ }
    fin = MPI_Wtime();
    printf("tiempo: %f\n", fin-inicio);

    FILE *fd;
    fd = fopen("program3.txt","a");
    fprintf(fd, "%d\t%f\n", size, fin - inicio);
    fclose(fd);
  }
 
  MPI_Finalize();
  
 
  return 0;
}

