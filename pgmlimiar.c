#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pgm.h"
#include "parser.h"
#include "arquivos.h"

int main(int argc, char **argv)
{
	FILE * image;

	char * input = NULL;
	char * output = NULL;
	
	//Valor real entre 0 e 1, define a porcentagem do valor maximo
	float limiar = 0.5;

	//Valor que define a porcentagem do valor max que sera o limar
	float max_limiar;

	//Propriedades do arquivo pgm
	char * tipo_arquivo;
	int col, lin, max;

	//Faz o parsing das entradas
	define_io(argc, argv, &input, &output, NULL, &limiar, NULL);

	//Le as propriedades do arquivo pgm, sem ler a matriz de pixels
	le_entradas(&image, input, &tipo_arquivo, &col, &lin, &max);

	//Inicia a struct pgm com as propriedades definidas
	pgm_t * pgm = inicializa_pgm(tipo_arquivo, col, lin, max);	

	//Copia a matriz de pixels da imagem para a struct pgm
	copia_matriz(pgm, &image, input);

	//Usuario digitou valor inválido para o limiar
	if (limiar < 0 || limiar > 1)
	{
		perror("Valor do limiar deve ser entre 0 e 1");
		return (-1);
	}
	
	//Define o valor do limiar
	max_limiar = limiar * pgm->max;

	//Aplica filtro de limiar na matriz de pixels
	for (int i = 0; i < pgm->lin; i++)
		for (int j = 0; j < pgm->col; j++)
		{
			//Valor do pixel eh maior ou igual ao limiar maximo, portanto, recebe o valor maximo da matriz
			if (pgm->matriz_pixels[i][j] >= max_limiar)
				pgm->matriz_pixels[i][j] = pgm->max;

			//Valor do pixel eh menor que o limiar maximo, portanto, recebe 0 (preto)
			else
				pgm->matriz_pixels[i][j] = 0;
		}

	//Copia o pgm com o filtro aplicado para algum arquivo de saida
	escreve_saidas(pgm, output);
}
