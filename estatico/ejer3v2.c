
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int first[500][500], second[500][500], multiply[500][500];

int main(int argc, char** argv)
{
  int m, c, d, k, rank, size;
  double fin, inicio;
 
  MPI_Status status;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  
  
  


  srand(time(NULL));

  

  if(rank == 0) {

    printf("Enter the number of rows and columns of first matrix\n");
    scanf("%d", &m);
    
    for (c = 0; c < m; c++)
      for (d = 0; d < m; d++)
        first[c][d] = 1 + rand() % 9;
        
    for (c = 0; c < m; c++)
        for (d = 0; d < m; d++)
          second[c][d] = 1 + rand() % 9;
  }
  
  inicio = MPI_Wtime();

  MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);//Envio de la dimension
  MPI_Bcast(&first, m*m, MPI_INT, 0, MPI_COMM_WORLD); //Envio y recepcion de la matriz A
  
  
  //~ if(rank == 0)  {
				//~ 
		//~ printf("\n");
		//~ printf("\n");
		//~ 
		//~ printf("Matriz A:\n");
		//~ for (c = 0; c < m; c++) {
		  //~ for (d = 0; d < m; d++)
			//~ printf("%d\t", first[c][d]);
			//~ 
			//~ printf("\n");
		//~ }
			//~ 
		//~ printf("\n");
		//~ printf("\n");
		//~ 
		//~ printf("Matriz B:\n");
		//~ for (c = 0; c < m; c++) {
		  //~ for (d = 0; d < m; d++)
			//~ printf("%d\t", second[c][d]);
			//~ 
			//~ printf("\n");
		//~ }
			//~ 
		//~ printf("\n");
		//~ printf("\n");
	//~ }
  
		
	
	
	// Enviamos Matriz B. Cada columna a un proceso.
	if(rank == 0)
		MPI_Scatter(&second[0][0], (m*m)/size, MPI_INT, MPI_IN_PLACE, (m*m)/size, MPI_INT, 0, MPI_COMM_WORLD);
	else
		MPI_Scatter(&second[0][0], (m*m)/size, MPI_INT, &second[0][0], (m*m)/size, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
    



  //Opera cada proceso su columna correspondiente
  for (c = 0; c < m; c++) {
    for (d = 0; d < m; d++) {
  		  multiply[c][d] = 0;
        for (k = 0; k < m; k++) {
          multiply[c][d] += first[c][k] * second[k][d];
          //~ printf("tmpC[%d] += ((%d*%d) = %d)\n", (c*m+d), first[c*m+k], tmpA[(d-rank)*m+k], tmpC[c*m+d]);
        }     
    }
  }


   // Devuelvo las columnas caluladas
	MPI_Barrier(MPI_COMM_WORLD);
	if(rank == 0)
		MPI_Gather(MPI_IN_PLACE, (m*m)/size, MPI_INT, &multiply[0][0], (m*m)/size, MPI_INT, 0, MPI_COMM_WORLD);
	else
		MPI_Gather(&multiply[0][0], (m*m)/size, MPI_INT, &multiply[0][0], (m*m)/size, MPI_INT, 0, MPI_COMM_WORLD);

  


  if(rank == 0) {
	fin = MPI_Wtime();
	
	//~ printf("\n");
	//~ printf("\n");
//~ 
    //~ printf("Product of entered matrices:\n");
//~ 
    //~ for (c = 0; c < m; c++) {
      //~ for (d = 0; d < m; d++)
        //~ printf("%d\t", multiply[c][d]);
//~ 
      //~ printf("\n");
    //~ }
    //~ 
    //~ printf("\n");
	//~ printf("\n");
    
    printf("tiempo: %f\n", fin-inicio);
  }
 
  MPI_Finalize();
  
 
  return 0;
}

