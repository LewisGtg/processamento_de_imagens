#include <stdio.h>
#include "pgm.h"
#include <stdlib.h>
#include <string.h>

//Funcao interna para copiar a matriz da imagem PGM do tipo P2 passada como argumento

pgm_t * inicializa_pgm(char tipo_arquivo[2+1], int col, int lin, int max)
{
	//Aloca memoria para o pgm e testa o malloc
	pgm_t * pgm = malloc(sizeof(pgm_t));

	if (!pgm)
	{
		perror("Falha ao inicializar tipo pgm");
		exit(1);
	}

	//Define as propriedades do PGM, e aloca uma matriz para os pixels
	strcpy(pgm->tipo_arquivo, tipo_arquivo);
	pgm->col = col;
	pgm->lin = lin;
	pgm->max = max;
	pgm->matriz_pixels = inicializa_matriz(col, lin);

	free(tipo_arquivo);

	return pgm;
}

void destroi_pgm(pgm_t * pgm)
{
	destroi_matriz(pgm->matriz_pixels);
	free(pgm);
}

int **inicializa_matriz(int col, int lin)
{
	//Aloca memoria para um vetor ponteiros
	int **matriz = malloc(sizeof(int*) * lin);

	//Aloca memoria para um vetor com todos os elementos da matriz
	matriz[0] = malloc(sizeof(int) * lin * col);

	//Testa os mallocs
	if (!matriz || !matriz[0])
	{
		perror("Falha ao alocar matriz");
		exit(1);
	}

	//Ajusta os demais ponteiros de linhas
	for (int i = 0; i < lin; i++)
		matriz[i] = matriz[0] + i * col;

	return matriz;
}

void destroi_matriz(int ** matriz)
{
	free(matriz[0]);
	free(matriz);
}


void info_pgm(pgm_t * pgm)
{
	printf("Tipo arquivo: %s\n", pgm->tipo_arquivo);
	printf("Tamanho arquivo: %d x %d\n", pgm->col, pgm->lin);
	printf("Valor maximo: %d\n", pgm->max);
}

int eh_arquivo_p2(pgm_t * pgm)
{
	return strcmp(pgm->tipo_arquivo, "P2") == 0;
}

int eh_arquivo_p5(pgm_t * pgm)
{
	return strcmp(pgm->tipo_arquivo, "P5") == 0;
}

void copia_matriz_pgm(pgm_t *pgm, int ** matriz)
{
	for (int i = 0; i < pgm->lin; i++)
		for (int j = 0; j < pgm->col; j++)
			matriz[i][j] = pgm->matriz_pixels[i][j];
}
