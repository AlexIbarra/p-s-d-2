#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

/* Genera una matriz y lo almacena en un fichero (en un formato particular, precedido por su dimensión) */
int main(int argc,char *argv[])
{
	if (argc != 4)
	{
		printf("Sintaxis: <nombre programa> <dimension1> <dimension2> <nombre fichero de matriz de salida>\n");
		exit(-1);
	}
	int b,filas,columnas,i,j,salida=0;
	time_t seed;
    	srand((unsigned) time(&seed));
	int a;
	FILE *fd;
	filas=atoi(argv[1]);
	columnas=atoi(argv[2]);
	fd=fopen(argv[3],"w");
	fprintf(fd,"%i %i ",filas,columnas);
	for(i=1;i<=filas;i++) 
		for(j=1;j<=columnas;j++) {
			b=(int)rand();
			a=b%200;
			a-=99;
			fprintf(fd,"%i ",a);
		}
	fclose(fd);
	printf("\n");
	return 0;
}
