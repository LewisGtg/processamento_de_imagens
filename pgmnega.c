/* LER OS ARGUMENTOS DE ENTRADA E DEFINIR SE O ARQUIVO A SER LIDO FOI PASSADO POR PARAMETRO, OU ESTA NA ENTRADA/SAIDA PADRAO

 LER O TIPO DE ARQUIVO PGM, SE FOR P2, TRATAR DE UMA FORMA, SE FOR P5, TRATAR DE OUTRA

 LER OS DADOS DO ARQUIVO PGM

 ARMAZENAR OS "PIXELS" EM UMA MATRIZ

 APLICAR O FILTRO DESEJADO NA MATRIZ

 COPIAR OS DADOS PARA O ARQUIVO DE SAIDA

 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pgm.h"
#include "parser.h"
#include "arquivos.h"

int main(int argc, char **argv)
{
	FILE * image;
	FILE * image_otp;

	char * input = NULL;
	char * output = NULL;

	char * tipo_arquivo;
	int col, lin, max;

	//Faz o parsing das entradas
	define_io(argc, argv, &input, &output);

	//Verifica se foi passado um arquivo de input como argumento
	le_entradas(&image, input, tipo_arquivo, &col, &lin, &max);
		
	//Inicia o arquivo pgm e copia sua matriz de pixels
	pgm_t * pgm = inicializa_pgm(tipo_arquivo, col, lin, max);	
	
	//Criar função para o bloco, (copia_pgm)
	copia_matriz(pgm, image, input);

	//Aplica o filtro na matriz de pixels
	for (int i = 0; i < pgm->lin; i++)
		for (int j = 0; j < pgm->col; j++)
			pgm->matriz_pixels[i][j] = pgm->max - pgm->matriz_pixels[i][j];

	if (output)
	{
		image_otp = fopen(output, "w+");

		if (!image_otp)
		{
			perror("Erro ao gerar arquivo de saida!");
			exit(1);
		}

		fprintf(image_otp, "%s\n", pgm->tipo_arquivo);
		fprintf(image_otp, "%d %d\n", pgm->col, pgm->lin);
		fprintf(image_otp, "%d\n", pgm->max);
		
		for (int i = 0; i < pgm->lin; i++)
		{
			for (int j = 0; j < pgm->col; j++)
				fprintf(image_otp, "%d ", pgm->matriz_pixels[i][j]);
			fprintf(image_otp, "\n");


		}
	}

}
