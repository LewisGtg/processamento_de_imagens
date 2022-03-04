#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "pgm.h"
#include "parser.h"
#include "arquivos.h"

void copia_matriz_pgm(pgm_t * pgm, int ** matriz);

int main(int argc, char **argv)
{
	FILE * image;

	char * input = NULL;
	char * output = NULL;
	float angulo;

	int ** matriz_copia;

	char * tipo_arquivo;
	int col, lin, max;

	double angulo_novo = 90;

	//Faz o parsing das entradas
	define_io(argc, argv, &input, &output, &angulo, NULL, NULL);
	//Verifica se foi passado um arquivo de input como argumento
	le_entradas(&image, input, &tipo_arquivo, &col, &lin, &max);

	//Inicia o arquivo pgm e copia sua matriz de pixels
	pgm_t * pgm = inicializa_pgm(tipo_arquivo, col, lin, max);	

	//Criar função para o bloco, (copia_pgm)
	copia_matriz(pgm, &image, input);

	matriz_copia = inicializa_matriz(col, lin);
	copia_matriz_pgm(pgm, matriz_copia);

	pgm->col = lin;
	pgm->lin = col;

	printf("%f\n", sin(angulo_novo));


	//Aplica o filtro negativo na matriz de pixels
	for (int x = 0; x < lin; x++)
	{
		for (int y = 0; y < col; y++)
		{
			int novo_x = x * cos(90) - y * sin(90);
			int novo_y = x * sin(90) + y * cos(90);

			//printf("novo_x = %d novo_y %d\nx = %d y = %d\n", novo_x, novo_y, x, y);
			if (novo_x >= 0 && novo_y >= 0)
			{
				pgm->matriz_pixels[novo_x][novo_y] = matriz_copia[x][y];
			}
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
