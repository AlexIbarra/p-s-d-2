#include <stdio.h>
//#include "mpi.h"
#include <stdlib.h>

int main(int argc, char** argv){

	clock_t start, end;
	double cpu_time_used;
	start = clock();

  int af, ac, i, j, k;

  printf("Introduce las dimensiones de la matriz A\n");
  scanf("%d", &af);
  scanf("%d", &ac);

  int A[af][ac], B[ac][af], C[af][af];

  for(i=0;i<af;i++){
      for(j=0;j<ac;j++){
	  A[i][j] = rand() % 11; //asigna un random del 0 al 10
      }
  }

  for(i=0;i<ac;i++){
      for(j=0;j<af;j++){
	  B[i][j] = rand() % 11; //asigna un random del 0 al 10
      }
  }

  for(i=0;i<af;i++){
      for(j=0;j<af;j++){
          C[i][j]=0;
          for(k=0;k<ac;k++){
              C[i][j]=(C[i][j]+(A[i][k]*B[k][j]));
          }
      }
  }
 
/*Rutina para imprimir*/
  printf("\n\n\t\t\t Matriz A");
  printf("\n");
  for(i=0;i<af;i++){
      printf("\n\t\t");
      for(j=0;j<ac;j++){
          printf("  %6d  ", A[i][j]);
      }
  }
  printf("\n\n\t\t\t Matriz B");
  printf("\n");
  for(i=0;i<ac;i++){
      printf("\n\t\t");
      for(j=0;j<af;j++){
          printf("  %6d  ", B[i][j]);
      }
  }
 
  printf("\n\n\t\t\t Matriz C");
  printf("\n");
  for(i=0;i<af;i++){
      printf("\n\t\t");
      for(j=0;j<af;j++){
          printf("  %6d  ", C[i][j]);
      }
  }
  printf("\n");
/*end = clock();
cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
printf("Tiempo invertido = %lf \n",cpu_time_used);*/
}
