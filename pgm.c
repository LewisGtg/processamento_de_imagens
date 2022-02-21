#include "pgm.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

pgm_t * inicializa_pgm(char tipo_arquivo[2+1], int col, int lin, int max)
{
	pgm_t * pgm = malloc(sizeof(pgm_t));

	if (!pgm)
	{
		perror("Falha ao inicializar tipo pgm");
		exit(1);
	}

	strcpy(pgm->tipo_arquivo, tipo_arquivo);
	pgm->col = col;
	pgm->lin = lin;
	pgm->max = max;
	pgm->matriz_pixels = inicializa_matriz(col, lin);

	return pgm;
}

int **inicializa_matriz(int col, int lin)
{
	int **matriz = malloc(sizeof(int*) * lin);

	matriz[0] = malloc(sizeof(int) * lin * col);

	if (!matriz || !matriz[0])
	{
		perror("Falha ao alocar matriz");
		exit(1);
	}

	for (int i = 0; i < lin; i++)
		matriz[i] = matriz[0] + i * col;

	return matriz;
}

void info_pgm(pgm_t * pgm)
{
	printf("Tipo arquivo: %s\n", pgm->tipo_arquivo);
	printf("Tamanho arquivo: %d x %d\n", pgm->col, pgm->lin);
	printf("Valor maximo: %d\n", pgm->max);
}
