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

	char * tipo_arquivo;
	int col, lin, max;

	//Faz o parsing das entradas
	parser(argc, argv, &input, &output, NULL, NULL, NULL);
	exit(1);

	//Le as propriedades do arquivo pgm, sem ler a matriz de pixels
	le_propriedades_pgm(&image, input, &tipo_arquivo, &col, &lin, &max);

	//Inicia a struct pgm com as propriedades definidas
	pgm_t * pgm = inicializa_pgm(tipo_arquivo, col, lin, max);	

	//Copia a matriz de pixels da imagem para a struct pgm
	le_matriz_pgm(pgm, &image, input);

	//Aplica o filtro negativo na matriz de pixels
	for (int i = 0; i < pgm->lin; i++)
		for (int j = 0; j < pgm->col; j++)
			pgm->matriz_pixels[i][j] = pgm->max - pgm->matriz_pixels[i][j];

	//Copia o pgm com o filtro aplicado para algum arquivo de saida
	gera_pgm(pgm, output);

	//Fecha o arquivo lido
	fecha_arquivo(image, input);

	//Fecha arquivo e desaloca estruturas usadas
	fecha_arquivo(image, input);
	destroi_pgm(pgm);
}
