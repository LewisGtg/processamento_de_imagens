#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "pgm.h"
#include "parser.h"
#include "arquivos.h"

#define PI 3.14159265 

void copia_matriz_pgm(pgm_t * pgm, int ** matriz);

//Retona 1 se a imagem for quadrada
int pgm_quadrado(pgm_t * pgm);

//Altera o tamanho da matriz de pixels do arquivo pgm
void redimensiona_pgm(pgm_t * pgm, int col, int lin);

//Identifica o maior e menor valor que um pixel pode assumir apos ser rotacionado
void identifica_limites(double angulo_rad, int col, int lin, int * max_col, int * max_lin, int * repara_col, int * repara_lin);

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
	define_io(argc, argv, &input, &output, &angulo_graus, NULL, NULL);
	angulo_rad = angulo_graus * PI / 180;

	//Verifica se foi passado um arquivo de input como argumento
	le_entradas(&image, input, &tipo_arquivo, &col, &lin, &max);

	//Inicia o arquivo pgm e copia sua matriz de pixels
	pgm_t * pgm = inicializa_pgm(tipo_arquivo, col, lin, max);	

	//Criar função para o bloco, (copia_pgm)
	copia_matriz(pgm, &image, input);

	matriz_copia = inicializa_matriz(col, lin);
	copia_matriz_pgm(pgm, matriz_copia);

	identifica_limites(angulo_rad, col, lin, &max_col, &max_lin, &repara_col, &repara_lin); 

	// Caso o pgm nao seja quadrado e o angulo de rotacao seja 90, apenas eh trocada linhas por colunas e vice versa
	if (!pgm_quadrado(pgm) && angulo_graus == 90)
		redimensiona_pgm(pgm, lin, col);

	// Caso o angulo nao seja de 90, redimensiona o pgm o valor minimo e maximo que um pixel pode assumir
	if (angulo_graus != 90)
		redimensiona_pgm(pgm, max_col, max_lin);


	//Aplica o filtro negativo na matriz de pixels
	for (int x = 0; x < lin; x++)
	{
		for (int y = 0; y < col; y++)
		{
			int novo_x = x * cos(angulo_rad) + y * sin(angulo_rad);
			int novo_y = x * -sin(angulo_rad) + y * cos(angulo_rad);

			pgm->matriz_pixels[novo_x + repara_lin][novo_y + repara_col] = matriz_copia[x][y];
		}
	}


	//Copia o pgm com o filtro aplicado para algum arquivo de saida
	escreve_saidas(pgm, output);
}

void copia_matriz_pgm(pgm_t *pgm, int ** matriz)
{
	for (int i = 0; i < pgm->lin; i++)
		for (int j = 0; j < pgm->col; j++)
			matriz[i][j] = pgm->matriz_pixels[i][j];
}

int pgm_quadrado(pgm_t * pgm)
{	
	return (pgm->lin == pgm->col);
}

void redimensiona_pgm(pgm_t * pgm, int col, int lin)
{
	free(pgm->matriz_pixels);

	pgm->lin = lin;
	pgm->col = col;

	pgm->matriz_pixels = inicializa_matriz(col, lin);

	for (int i = 0; i < pgm->lin; i++)
		for (int j = 0; j < pgm->col; j++)
			pgm->matriz_pixels[i][j] = 255;
}

void identifica_limites(double angulo_rad, int col, int lin, int * max_col, int * max_lin, int * repara_col, int * repara_lin)
{
	*max_col = 0;
	*max_lin = 0;
	*repara_col = 0;
	*repara_lin = 0;

	for (int x = 0; x < lin; x++)
		for (int y = 0; y < col; y++)
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

	*repara_col = abs(*repara_col);
	*repara_lin = abs(*repara_lin);

	*max_col += *repara_col + 1;
	*max_lin += *repara_lin + 1;
}
