#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int first[500][500], second[500][500], multiply[500][500];

int main(int argc, char** argv) {

  int i, m, c, d, k, rank, size;
  double fin, inicio;
  

  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int n_Columns[size],n_Rows[size],colums,rows,indices[size];
  

  srand(time(NULL));

  
  if(rank == 0) {

    //printf("Enter the number of rows and columns of first matrix\n");
    scanf("%d", &m);
    
    for (c = 0; c < m; c++)
      for (d = 0; d < m; d++)
        first[c][d] = 1 + rand() % 2;
        
    for (c = 0; c < m; c++)
      for (d = 0; d < m; d++)
        second[c][d] = 1 + rand() % 2;
  }
  


  if(rank == 0)  {
				
		printf("\n");
		printf("\n");
		
		printf("Matriz A:\n");
		for (c = 0; c < m; c++) {
		  for (d = 0; d < m; d++)
			printf("%d\t", first[c][d]);
			
			printf("\n");
		}
			
		printf("\n");
		printf("\n");
		
		printf("Matriz B:\n");
		for (c = 0; c < m; c++) {
		  for (d = 0; d < m; d++)
			printf("%d\t", second[c][d]);
			
			printf("\n");
		}
			
		printf("\n");
		printf("\n");
	}
  

  inicio = MPI_Wtime();

  MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);//Envio de la dimension
  MPI_Bcast(&second, m*m, MPI_INT, 0, MPI_COMM_WORLD); //Envio y recepcion de la matriz B
  
  
  /** DESCOMPOSICION DE LA MATRIZ  first */
  if(rank < m%size)
	rows = (m/size+1)*m;
  else
    rows = (m/size)*m;
    
  indices[0] = 0 ;
  
  if(0<m%size)
	n_Columns[0] = (m/size+1)*m;
  else
	n_Columns[0] = (m/size)*m;

	 
  for(i=1;i<size;i++){
    if(i<m%size)
	  n_Columns[i] = (m/size+1)*m;
    else
	  n_Columns[i] = (m/size)*m;
	  
    indices[i] = n_Columns[i-1]+indices[i-1];
  }

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Scatterv(&first,n_Columns,indices,MPI_INT,&first,rows,MPI_INT,0,MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);



  //Opera cada proceso su columna correspondiente
  for (c = 0; c < m; c++) {
    for (d = 0; d < m; d++) {
  	  multiply[c][d] = 0;
    	for (k = 0; k < m; k++) {
        multiply[c][d] += first[c][k] * second[k][d];
    	}     
    }
  }
	  
	  
	  /** AGRUPAMOS LOS RESULTADOS OBTENIDOS EN multiply */
  if(rank < m%size)
	colums = (m/size+1)*m;
  else
    colums = (m/size)*m;
    
  if(0<m%size)
	n_Rows[0] = (m/size+1)*m;
  else
	n_Rows[0] = (m/size)*m;
	
  indices[0] = 0;


  for(i=1;i<size;i++) {
    if(i<m%size)
	  n_Rows[i] = (m/size+1)*m;
    else
	  n_Rows[i] = (m/size)*m;
	  
    indices[i] = n_Rows[i-1]+indices[i-1];
  }


   // Devuelvo las columnas caluladas
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Gatherv(&multiply,colums,MPI_INT,&multiply,n_Rows,indices,MPI_INT,0,MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  


  if(rank == 0) {
    fin = MPI_Wtime();

    printf("\n");
    printf("\n");

    printf("Product of entered matrices:\n");

    for (c = 0; c < m; c++) {
      for (d = 0; d < m; d++)
        printf("%d\t", multiply[c][d]);

      printf("\n");
    }
    
    printf("\n");
    printf("\n");
    
    printf("tiempo: %f\n", fin-inicio);

    FILE *fd;
    fd = fopen("program4.txt","a");
    fprintf(fd, "%d\t%f\n", size, fin - inicio);
    fclose(fd);
  }
 
  MPI_Finalize();
  
 
  return 0;
}


