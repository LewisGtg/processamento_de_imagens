#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pgm.h"
#include "parser.h"
#include "arquivos.h"

//Verifica se a celula (pixel) da matriz em dado endereco esta no meio, ou seja, em nenhuma borda
int pixel_central(pgm_t * pgm, int lin, int col);

//Verifica se o pixel em dado endereco esta na borda superior
int pixel_borda_sup(pgm_t * pgm, int lin, int col);

//Verifica se o pixel em dado endereco esta na borda inferior
int pixel_borda_inf(pgm_t * pgm, int lin, int col);

//Verifica se o pixel em dado endereco esta na borda direita
int pixel_borda_dir(pgm_t * pgm, int lin, int col);

//Verifica se o pixel em dado endereco esta na borda esquerda
int pixel_borda_esq(pgm_t * pgm, int lin, int col);

//Aplica filtro em pixels centrais
void filtro_central(pgm_t * pgm, int ** matriz_copia, int lin, int col);

//Aplica filtro em pixels da borda superior
void filtro_borda_sup(pgm_t * pgm, int ** matriz_copia, int lin, int col);

//Aplica filtro em pixels da borda inferior
void filtro_borda_inf(pgm_t * pgm, int ** matriz_copia, int lin, int col);

//Aplica filtro em pixels da borda esquerda
void filtro_borda_esq(pgm_t * pgm, int ** matriz_copia, int lin, int col);

//Aplica filtro em pixels da borda direita
void filtro_borda_dir(pgm_t * pgm, int ** matriz_copia, int lin, int col);

int main(int argc, char **argv)
{
	FILE * image;

	char * input = NULL;
	char * output = NULL;

	int ** matriz_copia;

	//Propriedades do arquivo pgm
	char * tipo_arquivo;
	int col, lin, max;

	//Faz o parsing das entradas
	parser(argc, argv, &input, &output, NULL, NULL, NULL);

	//Le as propriedades do arquivo pgm, sem ler a matriz de pixels
	le_propriedades_pgm(&image, input, &tipo_arquivo, &col, &lin, &max);

	//Inicia a struct pgm com as propriedades definidas
	pgm_t * pgm = inicializa_pgm(tipo_arquivo, col, lin, max);	

	//Copia a matriz de pixels para a struct pgm
	le_matriz_pgm(pgm, &image, input);

	//Cria uma copia para a matriz de pixels do pgm
	matriz_copia = inicializa_matriz(col, lin);
	copia_matriz_pgm(pgm, matriz_copia);

	//Aplica a reducao de ruido na matriz de pixels
	for (int i = 0; i < pgm->lin; i++)
		for (int j = 0; j < pgm->col; j++)
		{
			//Pixel nao esta em nenhuma borda da matriz
			if (pixel_central(pgm, i, j))
				filtro_central(pgm, matriz_copia, i, j);

			//Pixel esta na borda superior da matriz
			if (pixel_borda_sup(pgm, i, j))
				filtro_borda_sup(pgm, matriz_copia, i, j);

			//Pixel esta na borda inferior da matriz
			if (pixel_borda_inf(pgm, i, j))
				filtro_borda_inf(pgm, matriz_copia, i, j);

			//Pixel esta na borda esquerda, mas nao esta em nenhuma extremidade vertical
			if (pixel_borda_esq(pgm, i, j))
				filtro_borda_esq(pgm, matriz_copia, i, j);

			//Pixel esta na borda direita, mas nao esta em nenhuma extremidade vertical
			if (pixel_borda_dir(pgm, i, j))
				filtro_borda_dir(pgm, matriz_copia, i, j);
		}

	//Copia o pgm com o filtro aplicado para algum arquivo de saida
	gera_pgm(pgm, output);

	
	//Fecha arquivo e desaloca estruturas usadas
	fecha_arquivo(image, input);
	destroi_matriz(matriz_copia);
	destroi_pgm(pgm);
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

void filtro_central(pgm_t * pgm, int ** matriz_copia, int i, int j)
{
	//Seleciona todos os valores vizinhos ao pixel
	int linha_sup = matriz_copia[i-1][j-1] + matriz_copia[i-1][j] + matriz_copia[i-1][j+1];
	int linha_inf = matriz_copia[i+1][j-1] + matriz_copia[i+1][j] + matriz_copia[i+1][j+1];
	int dir = matriz_copia[i][j+1];
	int esq = matriz_copia[i][j-1];

	//Aplica o filtro de media no pixel
	pgm->matriz_pixels[i][j] = (matriz_copia[i][j] + linha_sup + linha_inf + dir + esq) / 9;
}

void filtro_borda_sup(pgm_t * pgm, int ** matriz_copia, int i, int j)
{
	//Pixel superior esquerdo
	if (j == 0)
		//Aplica o filtro com a media dos vizinhos possiveis
		pgm->matriz_pixels[i][j] = (matriz_copia[i][j] + matriz_copia[i][j+1] + matriz_copia[i+1][j+1] + matriz_copia[i+1][j]) / 4;

	//Pixel superior direito
	else if (j == pgm->col - 1)
		//Aplica o filtro com a media dos vizinhos possiveis
		pgm->matriz_pixels[i][j] = (matriz_copia[i][j] + matriz_copia[i][j-1] + matriz_copia[i+1][j-1] + matriz_copia[i+1][j]) / 4;

	//Pixel esta na borda superior, mas nao esta em nenhuma extremidade horizontal
	else
	{
		//Seleciona todos os valores dos vizinhos possiveis
		int linha_inf = matriz_copia[i+1][j-1] + matriz_copia[i+1][j] + matriz_copia[i+1][j+1];
		int dir = matriz_copia[i][j+1];
		int esq = matriz_copia[i][j-1];

		//Aplica o filtro de media no pixel
		pgm->matriz_pixels[i][j] = (matriz_copia[i][j] + linha_inf + dir + esq) / 6;
	}
}

void filtro_borda_inf(pgm_t * pgm, int ** matriz_copia, int i, int j)
{
	//Pixel inferior esquerdo
	if (j == 0)
		//Aplica o filtro com a media dos vizinhos possiveis
		pgm->matriz_pixels[i][j] = (matriz_copia[i][j] + matriz_copia[i-1][j] + matriz_copia[i-1][j+1] + matriz_copia[i][j+1]) / 4;

	//Pixel inferior direito
	else if (j == pgm->col - 1)
		//Aplica o filtro com a media dos vizinhos possiveis
		pgm->matriz_pixels[i][j] = (matriz_copia[i][j] + matriz_copia[i][j-1] + matriz_copia[i-1][j-1] + matriz_copia[i-1][j]) / 4;

	//Pixel esta na borda inferior, mas nao esta em nenhuma extremidade horizontal
	else
	{
		//Seleciona todos os valores dos vizinhos possiveis
		int linha_sup = matriz_copia[i-1][j-1] + matriz_copia[i-1][j] + matriz_copia[i-1][j+1];
		int dir = matriz_copia[i][j+1];
		int esq = matriz_copia[i][j-1];

		//Aplica o filtro de media no pixel
		pgm->matriz_pixels[i][j] = (matriz_copia[i][j] + linha_sup + dir + esq) / 6;
	}
}

void filtro_borda_esq(pgm_t * pgm, int ** matriz_copia, int i, int j)
{
	//Seleciona todos os valores dos vizinhos possiveis
	int linha_dir = matriz_copia[i-1][j+1] + matriz_copia[i][j+1] + matriz_copia[i+1][j+1];
	int cima = matriz_copia[i-1][j];
	int baixo = matriz_copia[i+1][j];

	//Aplica o filtro de media no pixel
	pgm->matriz_pixels[i][j] = (matriz_copia[i][j] + linha_dir + cima + baixo) / 6;
}

void filtro_borda_dir(pgm_t * pgm, int ** matriz_copia, int i, int j)
{
	//Seleciona todos os valores dos vizinhos possiveis
	int linha_esq = matriz_copia[i-1][j-1] + matriz_copia[i][j-1] + matriz_copia[i+1][j-1];
	int cima = matriz_copia[i-1][j];
	int baixo = matriz_copia[i+1][j];

	//Aplica o filtro de media no pixel
	pgm->matriz_pixels[i][j] = (matriz_copia[i][j] + linha_esq + cima + baixo) / 6;
}
