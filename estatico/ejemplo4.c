#include<stdio.h>
#include<mpi.h>
#define M 150
#define N 160
#define P 165
/**  MASTER WORKER  */
int main(int argc,char*argv[]){
	 
	 int rank,size;
	 
	 MPI_Status status;
	 MPI_Init(&argc,&argv);
	 MPI_Comm_size(MPI_COMM_WORLD,&size);
	 MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	 
	 int A[M][N],B[N][P],C[M][P];
	 int i,j,k;
	 
	 
	 if(rank == 0){
		/** LEEMOS LA MATRIZ A Y B EN EL PROCESO 0*/
	 }
	 
	 MPI_Barrier(MPI_COMM_WORLD);
	 
	 double t0 = MPI_Wtime();
	 int n_Columns[size],n_Rows[size],colums,rows,indices[size];
	 
	 /** MANDAMOS LA MATRIZ B A TODOS LOS PROCESOS*/
	 MPI_Bcast(B,N*P,MPI_INT,0,MPI_COMM_WORLD);
	 
	 /** DESCOMPOSICION DE LA MATRIZ  A */
	 rows = ( rank < M%size )?(M/size+1)*N:(M/size)*N;
	 indices[0] = 0 ;
	 n_Columns[0] = ( 0<M%size )?(M/size+1)*N:(M/size)*N;
	 
	 for(i=1;i<size;i++){
		  n_Columns[i] = (i < M%size ) ? (M/size+1)*N:(M/size)*N;
		  indices[i] = n_Columns[i-1]+indices[i-1];
	 }
	 
	 MPI_Barrier(MPI_COMM_WORLD);
	 MPI_Scatterv(A,n_Columns,indices,MPI_INT,A,rows,MPI_INT,0,MPI_COMM_WORLD);
	 MPI_Barrier(MPI_COMM_WORLD);
	 
	 /** MULTIPLICACIÃ“N DE LA MATRIZ B con Bloques de A */
	 for(i=0;i<rows/N;i++)
	  for(j=0;j<P;j++){
	   C[i][j] = 0;
	   for(k=0;k<N;k++)
		C[i][j] += A[i][k]*B[k][j];
	  }


	 /** AGRUPAMOS LOS RESULTADOS OBTENIDOS EN C */
	 colums = ( rank < M%size )?(M/size+1)*P : M/size*P;
	 n_Rows[0] = ( 0 < M%size )?(M/size+1)*P : M/size*P;
	 indices[0] = 0;
	 
	 for(i=1;i<size;i++){
		  n_Rows[i] = (i < M%size )?(M/size+1)*P:(M/size)*P;
		  indices[i] = n_Rows[i-1]+indices[i-1];
	 }

	 MPI_Barrier(MPI_COMM_WORLD);
	 MPI_Gatherv(C,colums,MPI_INT,C,n_Rows,indices,MPI_INT,0,MPI_COMM_WORLD);
	 MPI_Barrier(MPI_COMM_WORLD);
	 double t1 = MPI_Wtime();
	 
	 /** FIN DEL ALGORITMO SIMPLON */
	 if( rank == 0 ){
		  printf(" MATRIX A[%d][%d]XB[%d][%d]=C[%d][%d]\n",M,N,N,P,M,P);
		  /*for(i=0;i<M;i++){
		   for(j=0;j<P;j++)
			printf(" %d ",C[i][j]);
		   printf("\n");
		  }*/
		  printf("time %f!\n",t1-t0);
	 }
	 MPI_Barrier(MPI_COMM_WORLD);
	 MPI_Finalize();
	 return 0;
}
