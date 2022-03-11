#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "pgm.h"
#include "parser.h"
#include "arquivos.h"

#define PI 3.14159265 

//Retona 1 se a imagem for quadrada
int pgm_quadrado(pgm_t * pgm);

//Verifica se um angulo é multiplo de 90
int eh_mult_noventa(float angulo);

//Altera o tamanho da matriz de pixels do arquivo pgm
void redimensiona_pgm(pgm_t * pgm, int col, int lin);

//Muda o valor de todos os pixels para -1
void desliga_pixels(pgm_t * pgm);

//Identifica o maior e menor valor que um pixel pode assumir apos ser rotacionado
void identifica_limites(pgm_t * pgm, double angulo_rad, int * max_col, int * max_lin, int * repara_col, int * repara_lin);

int main(int argc, char **argv)
{
	FILE * image;

	char * input = NULL;
	char * output = NULL;

	double angulo_graus = 90;
	double angulo_rad;
	int max_col, max_lin, repara_col, repara_lin;

	int ** matriz_copia;

	char * tipo_arquivo;
	int col, lin, max;

	//Faz o parsing das entradas
	parser(argc, argv, &input, &output, &angulo_graus, NULL, NULL);
	angulo_rad = angulo_graus * PI / 180;

	//Le as propriedades do arquivo pgm, sem ler a matriz de pixels
	le_propriedades_pgm(&image, input, &tipo_arquivo, &col, &lin, &max);

	//Inicia a struct pgm com as propriedades definidas
	pgm_t * pgm = inicializa_pgm(tipo_arquivo, col, lin, max);	

	//Copia a matriz de pixels da imagem para a struct pgm
	le_matriz_pgm(pgm, &image, input);

	matriz_copia = inicializa_matriz(col, lin);
	copia_matriz_pgm(pgm, matriz_copia);

	identifica_limites(pgm, angulo_rad, &max_col, &max_lin, &repara_col, &repara_lin); 

	// Caso o pgm nao seja quadrado e o angulo de rotacao seja multiplo de 90, apenas eh trocada linhas por colunas e vice versa
	if (!pgm_quadrado(pgm) && eh_mult_noventa(angulo_graus))
		redimensiona_pgm(pgm, lin, col);


	// Caso o angulo nao seja de multiplo de 90, redimensiona o pgm com o valor minimo e maximo que um pixel pode assumir
	if (!eh_mult_noventa(angulo_graus))
	{
		redimensiona_pgm(pgm, max_col, max_lin);
		desliga_pixels(pgm);
	}

	//Aplica o filtro de rotacao 
	if (!eh_mult_noventa(angulo_graus))
	{
		for (int x = 0; x < lin; x++)
		{
			for (int y = 0; y < col; y++)
			{
				int novo_x = x * cos(angulo_rad) + y * sin(angulo_rad);
				int novo_y = x * -sin(angulo_rad) + y * cos(angulo_rad);

				pgm->matriz_pixels[novo_x + repara_lin][novo_y + repara_col] = matriz_copia[x][y];
			}
		}
	}

	identifica_limites(pgm, -angulo_rad, &max_col, &max_lin, &repara_col, &repara_lin); 
	for (int x = 0; x < pgm->lin; x++)
	{
		for (int y = 0; y < pgm->col; y++)
		{
			if (pgm->matriz_pixels[x][y] == -1)
				pgm->matriz_pixels[x][y] = 255;
			else
			{
				int antigo_x = x * cos(-angulo_rad) + y * sin(-angulo_rad);
				int antigo_y = x * fabs(-sin(-angulo_rad)) + y * fabs(cos(-angulo_rad));

				printf("%d %d %d %d \n", antigo_x, antigo_y, x, repara_col);

				//pgm->matriz_pixels[x][y] = matriz_copia[x][y];
			}
		}
	}

	if (!eh_mult_noventa(angulo_graus))
	{
		for (int x = 0; x < pgm->lin; x++)
			for (int y = 0; y < pgm->col; y++)
			{
				if (pgm->matriz_pixels[x][y] == -1)
					pgm->matriz_pixels[x][y] = 255;
				else
				{
					
					

				}
			}
	}

	//Copia o pgm com o filtro aplicado para algum arquivo de saida
	//gera_pgm(pgm, output);

	//Fecha arquivo e desaloca estruturas usadas
	fecha_arquivo(image, input);
	destroi_matriz(matriz_copia);
	destroi_pgm(pgm);
}


int pgm_quadrado(pgm_t * pgm)
{	
	return (pgm->lin == pgm->col);
}

int eh_mult_noventa(float angulo)
{
	return ((int) angulo % 90 == 0);
}

void redimensiona_pgm(pgm_t * pgm, int col, int lin)
{
	free(pgm->matriz_pixels[0]);
	free(pgm->matriz_pixels);

	pgm->lin = lin;
	pgm->col = col;

	pgm->matriz_pixels = inicializa_matriz(col, lin);
}

void desliga_pixels(pgm_t * pgm)
{
	for (int i = 0; i < pgm->lin; i++)
		for (int j = 0; j < pgm->col; j++)
			pgm->matriz_pixels[i][j] = -1;
}

void identifica_limites(pgm_t *pgm, double angulo_rad, int * max_col, int * max_lin, int * repara_col, int * repara_lin)
{
	*max_col = 0;
	*max_lin = 0;
	*repara_col = 0;
	*repara_lin = 0;

	for (int x = 0; x < pgm->lin; x++)
		for (int y = 0; y < pgm->col; y++)
		{
			if (pgm->matriz_pixels[x][y] != -1)
			{
				int novo_x = x * cos(angulo_rad) + y * sin(angulo_rad);
				int novo_y = x * -sin(angulo_rad) + y * cos(angulo_rad);

				if (novo_x >= *max_lin)
					*max_lin = novo_x + 1;

				if (novo_x <= *repara_lin)
					*repara_lin = novo_x;

				if (novo_y <= *repara_col)
					*repara_col = novo_y;

				if (novo_y >= *max_col)
					*max_col = novo_y;
			}
		}

	*repara_col = abs(*repara_col);
	*repara_lin = abs(*repara_lin);

	*max_col += *repara_col + 1;
	*max_lin += *repara_lin + 1;
}
