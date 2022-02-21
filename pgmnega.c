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
#include "arquivo.h"

int main(int argc, char **argv)
{
	FILE * image;
	char * input = NULL;
	char * output = NULL;
	char tipo_arquivo[2+1];
	int col, lin, max;
	int option;

	//Trata os argumentos de entrada (fazer funcao para isso)
	while ((option = getopt(argc, argv, "i:o:")) != -1)
	{
		switch(option)
		{
			case 'i':
				input = optarg;
				break;
			
			case 'o':
				output = optarg;
				break;
			
			default:
				fprintf(stderr, "Usage: -i input -o output\n");
				exit(1);
		}
	}

	//Verifica se foi passado um arquivo de input como argumento
	if (input)
	{
		//Abre a imagem e pega os dados do pgm (criar funcao)
		image = fopen(input, "r");

		if (!image)
		{
				perror("Erro ao abrir arquivo de imagem");
				exit(1);
		}
		
		//Leitura de arquivos P2
		fscanf(image, "%s", tipo_arquivo);
		fscanf(image, "%d", &col);
		fscanf(image, "%d", &lin);
		fscanf(image, "%d", &max);
	}

	else
	{
		//Pega os dados da imagem da entrada padrao (criar funcao)
		fscanf(stdin, "%s", tipo_arquivo);
		fscanf(stdin, "%d", &col);
		fscanf(stdin, "%d", &lin);
		fscanf(stdin, "%d", &max);
	}

	pgm_t * pgm = inicializa_pgm(tipo_arquivo, col, lin, max);	

	info_pgm(pgm);
	
	if (eh_arquivo_p2(pgm))
		copia_matriz_p2(pgm, image);

	if (eh_arquivo_p5(pgm))
		copia_matriz_p5(pgm, image);

	

}
