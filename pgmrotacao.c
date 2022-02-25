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
	int angulo;

	char * tipo_arquivo;
	int col, lin, max;

	//Faz o parsing das entradas
	define_io(argc, argv, &input, &output, &angulo, NULL, NULL);
	//Verifica se foi passado um arquivo de input como argumento
	le_entradas(&image, input, &tipo_arquivo, &col, &lin, &max);

	//Inicia o arquivo pgm e copia sua matriz de pixels
	pgm_t * pgm = inicializa_pgm(tipo_arquivo, col, lin, max);	

	//Criar função para o bloco, (copia_pgm)
	copia_matriz(pgm, &image, input);

	//Aplica o filtro negativo na matriz de pixels

	//Copia o pgm com o filtro aplicado para algum arquivo de saida
	escreve_saidas(pgm, output);
}
