#ifndef SRC_MATRIXLIB_H_
#define SRC_MATRIXLIB_H_

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>

void leerMatriz(int*** array, int* filas, int* columnas, FILE *fd);
void imprimirMatriz(int** array, int filas, int columnas);
void escribirMatriz(int** array, int filas, int columnas, FILE *fd);
void escribirUnidadI(int i);

#endif /* SRC_MATRIXLIB_H_ */

