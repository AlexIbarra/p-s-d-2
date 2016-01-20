
#include "matrixLib.h"

int main(int argc,char *argv[]){

	if (argc!=4){
		printf("Has introducido %i parametros.\n",argc);
		printf("Sintaxis: <nombre programa> <archivo de matriz 1> <archivo de matriz 2> <archivo de salida>\n");
		exit(1);
	}

	FILE *fEntrada1, *fEntrada2, *fSalida;
	int** mEntrada1;
	int** mEntrada2 = NULL;

	//Dimensiones de las matrices
	int filasA, columnasA, filasB, columnasB;

	fEntrada1=fopen(argv[1],"r");
	fEntrada2=fopen(argv[2],"r");
	fSalida=fopen(argv[3],"w");

	if(!fEntrada1 || !fEntrada2 || !fSalida){
		printf("Error abriendo ficheros. Compruebe que el fichero de entrada existe\r\n");
		exit(1);
	}
	//Lee los valores de las dos matrices y cerrar los ficheros correspondientes
	leerMatriz(&mEntrada1, &filasA, &columnasA, fEntrada1);
	leerMatriz(&mEntrada2, &filasB, &columnasB, fEntrada2);
	fclose(fEntrada1);
	fclose(fEntrada2);

	if((filasA!=columnasB)||(columnasA!=filasB)){
		printf("\n\n*********Matrices incompatibles************\r\n");
		printf("Filas de A: %d; Columnas de A: %d;\r\nFilas de B: %d; Columnas de B: %d;\r\n ", filasA, columnasA, filasB, columnasB);
		fclose(fSalida);
		exit(1);
	}

	int i, j, k, sum=0;
	int* offset;

	int** resultado = malloc(filasA*(sizeof(int*)+columnasB*sizeof(int)));
        offset = (int*) resultado + filasA;
	for(i=0; i<filasA; i++, offset+=columnasB)
		resultado[i]=offset;

	for(i = 0; i < filasA; i++)
		for(j = 0; j < columnasB; j++){
			sum = 0;
			for(k=0; k < filasB; k++)
				sum += mEntrada1[i][k]*mEntrada2[k][j];
			resultado[i][j] = sum;
		}

	printf("Matriz 1: \r\n");
	imprimirMatriz(mEntrada1, filasA, columnasA);
	printf("Matriz 2: \r\n");
	imprimirMatriz(mEntrada2, filasB, columnasB);
	printf("Resultado: \r\n");
	imprimirMatriz(resultado, filasA, columnasB);

	escribirMatriz(resultado, filasA, columnasB, fSalida);
	fclose(fSalida);

	free(mEntrada1);
	free(mEntrada2);
	free(resultado);
	return 0;
}

