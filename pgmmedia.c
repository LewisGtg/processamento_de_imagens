#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pgm.h"
#include "parser.h"
#include "arquivos.h"

void copia_matriz_pgm(pgm_t * pgm, int ** matriz);

int pixel_central(pgm_t * pgm, int lin, int col);

int pixel_borda_sup(pgm_t * pgm, int lin, int col);

int pixel_borda_inf(pgm_t * pgm, int lin, int col);

int pixel_borda_dir(pgm_t * pgm, int lin, int col);

int pixel_borda_esq(pgm_t * pgm, int lin, int col);

int main(int argc, char **argv)
{
	FILE * image;

	char * input = NULL;
	char * output = NULL;
	float media;
	int ** matriz_copia;

	char * tipo_arquivo;
	int col, lin, max;

	//Faz o parsing das entradas
	define_io(argc, argv, &input, &output, NULL, NULL, &media);

	//Le as propriedades do arquivo pgm, sem ler a matriz de pixels
	le_entradas(&image, input, &tipo_arquivo, &col, &lin, &max);

	//Inicia a struct pgm com as propriedades definidas
	pgm_t * pgm = inicializa_pgm(tipo_arquivo, col, lin, max);	

	//Copia a matriz de pixels para a struct pgm
	copia_matriz(pgm, &image, input);

	matriz_copia = inicializa_matriz(col, lin);
	copia_matriz_pgm(pgm, matriz_copia);

	//Aplica a reducao de ruido na matriz de pixels
	for (int i = 0; i < pgm->lin; i++)
		for (int j = 0; j < pgm->col; j++)
		{
			//Pixel nao esta em nenhuma borda da matriz
			if (pixel_central(pgm, i, j))
			{
				int linha_sup = matriz_copia[i-1][j-1] + matriz_copia[i-1][j] + matriz_copia[i-1][j+1];
				int linha_inf = matriz_copia[i+1][j-1] + matriz_copia[i+1][j] + matriz_copia[i+1][j+1];
				int dir = matriz_copia[i][j+1];
				int esq = matriz_copia[i][j-1];

				pgm->matriz_pixels[i][j] = (matriz_copia[i][j] + linha_sup + linha_inf + dir + esq) / 9;
			}	

			//Pixel esta na borda superior da matriz
			if (pixel_borda_sup(pgm, i, j))
			{	
				//Pixel superior esquerdo
				if (j == 0)
					pgm->matriz_pixels[i][j] = (matriz_copia[i][j] + matriz_copia[i][j+1] + matriz_copia[i+1][j+1] + matriz_copia[i+1][j]) / 4;

				//Pixel superior direito
				else if (j == pgm->col - 1)
					pgm->matriz_pixels[i][j] = (matriz_copia[i][j] + matriz_copia[i][j-1] + matriz_copia[i+1][j-1] + matriz_copia[i+1][j]) / 4;

				//Pixel esta na borda superior, mas nao esta em nenhuma extremidade horizontal
				else
				{
					int linha_inf = matriz_copia[i+1][j-1] + matriz_copia[i+1][j] + matriz_copia[i+1][j+1];
					int dir = matriz_copia[i][j+1];
					int esq = matriz_copia[i][j-1];

					pgm->matriz_pixels[i][j] = (matriz_copia[i][j] + linha_inf + dir + esq) / 6;
				}

			}

			//Pixel esta na borda inferior da matriz
			if (pixel_borda_inf(pgm, i, j))
			{
				//Pixel inferior esquerdo
				if (j == 0)
					pgm->matriz_pixels[i][j] = (matriz_copia[i][j] + matriz_copia[i-1][j] + matriz_copia[i-1][j+1] + matriz_copia[i][j+1]) / 4;

				//Pixel inferior direito
				else if (j == pgm->col - 1)
					pgm->matriz_pixels[i][j] = (matriz_copia[i][j] + matriz_copia[i][j-1] + matriz_copia[i-1][j-1] + matriz_copia[i-1][j]) / 4;

				//Pixel esta na borda inferior, mas nao esta em nenhuma extremidade horizontal
				else
				{
					int linha_sup = matriz_copia[i-1][j-1] + matriz_copia[i-1][j] + matriz_copia[i-1][j+1];
					int dir = matriz_copia[i][j+1];
					int esq = matriz_copia[i][j-1];

					pgm->matriz_pixels[i][j] = (matriz_copia[i][j] + linha_sup + dir + esq) / 6;
				}
			}

			//Pixel esta na borda esquerda, mas nao esta em nenhuma extremidade vertical
			if (pixel_borda_esq(pgm, i, j))
			{
				int linha_dir = matriz_copia[i-1][j+1] + matriz_copia[i][j+1] + matriz_copia[i+1][j+1];
				int cima = matriz_copia[i-1][j];
				int baixo = matriz_copia[i+1][j];

				pgm->matriz_pixels[i][j] = (matriz_copia[i][j] + linha_dir + cima + baixo) / 6;

			}

			if (pixel_borda_dir(pgm, i, j))
			{
				int linha_esq = matriz_copia[i-1][j-1] + matriz_copia[i][j-1] + matriz_copia[i+1][j-1];
				int cima = matriz_copia[i-1][j];
				int baixo = matriz_copia[i+1][j];

				pgm->matriz_pixels[i][j] = (matriz_copia[i][j] + linha_esq + cima + baixo) / 6;
			}
		}

	//Copia o pgm com o filtro aplicado para algum arquivo de saida
	escreve_saidas(pgm, output);
}

void copia_matriz_pgm(pgm_t * pgm, int ** matriz)
{
	for (int i = 0; i < pgm->lin; i++)
		for (int j = 0; j < pgm->col; j++)
			matriz[i][j] = pgm->matriz_pixels[i][j];
}

int pixel_central(pgm_t * pgm, int i, int j)
{	
	return (i > 0 && i < (pgm->lin - 1)) && (j > 0 && j < (pgm->col - 1));
}

int pixel_borda_sup(pgm_t * pgm, int i, int j)
{
	return (i == 0);
}

int pixel_borda_inf(pgm_t * pgm, int i, int j)
{
	return (i == pgm->lin - 1);
}

int pixel_borda_esq(pgm_t * pgm, int i, int j)
{
	return (j == 0 && i > 1 && (i < pgm->lin - 1));
}

int pixel_borda_dir(pgm_t * pgm, int i, int j)
{
	return ((j == pgm->col - 1) && i > 1 && (i < pgm->lin - 1));
}
