
#include "matrixLib.h"

/* Escribir un int de la matriz en una posicon que de un formato legible */
void escribirUnidadI(int i){
	if(i<-9)
		printf("%d ",i);
	else
		if((i<10)&&(i>=0))
			printf("  %d ",i);
		else
			printf(" %d ",i);
}

/* Escribe una matriz por pantalla con formato, usando la funcion escribeUnidad */
void imprimirMatriz(int** array, int filas, int columnas)
{
	int i,j;
	for(i = 0; i < (columnas-1); i++)
		printf("----");
	printf("---\n");
	for(i = 0; i < filas; i++){
		for(j = 0; j < columnas; j++)
			escribirUnidadI(array[i][j]);
		printf("\n");
	}
	for(i = 0; i < (columnas-1); i++)
		printf("----");
	printf("---\n");
}

/* Escribe el array en el fichero */
void escribirMatriz(int** array, int filas, int columnas, FILE *fd)
{
	int i,j;
	fprintf(fd,"%i %i", filas, columnas);
	for(i = 0; i < filas; i++)
		for(j=0; j < columnas; j++)
			fprintf(fd," %d",array[i][j]);
}

/* Lee del fichero de entrada una matriz y la deja en un array. */
void leerMatriz(int*** array, int* filas, int* columnas, FILE *fd)
{
	int i, j, filasFichero, columnasFichero, intLeido;
        fscanf(fd, "%d%d", &filasFichero, &columnasFichero);
	
	int** arrayFichero = malloc(filasFichero*(sizeof(int*) + columnasFichero*sizeof(int)));
        int *offset = (int*) arrayFichero + filasFichero;
	for(i=0; i<filasFichero; i++, offset += columnasFichero)
		arrayFichero[i] = offset;
	
	for(i=0; i<filasFichero; i++)
		for(j=0; j<columnasFichero; j++){
			fscanf(fd,"%d", &intLeido);
                        arrayFichero[i][j]=intLeido;
		}
	*array = arrayFichero;
        *filas = filasFichero;
	*columnas = columnasFichero; 
}

