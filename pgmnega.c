/* LER OS ARGUMENTOS DE ENTRADA E DEFINIR SE O ARQUIVO A SER LIDO FOI PASSADO POR PARAMETRO, OU ESTA NA ENTRADA/SAIDA PADRAO

 LER O TIPO DE ARQUIVO PGM, SE FOR P2, TRATAR DE UMA FORMA, SE FOR P5, TRATAR DE OUTRA

 LER OS DADOS DO ARQUIVO PGM

 ARMAZENAR OS "PIXELS" EM UMA MATRIZ

 APLICAR O FILTRO DESEJADO NA MATRIZ

 COPIAR OS DADOS PARA O ARQUIVO DE SAIDA

 */

#include <stdio.h>
#include <unistd.h>
#include "pgm.h"

int main(int argc, char **argv)
{
	FILE * image;
	char * input = NULL;
	char * output = NULL;
	char tipo_arquivo[2+1];
	int col, lin, max;
	
	define_io(argc, argv, input, output)

	image = fopen(input, "r");

	if (!image)
	{
		perror("Erro ao abrir arquivo de imagem");
		exit(1);
	}

	//Leitura de arquivos P2
	fscanf(image, "%s", tipo_arquivo);
	fscanf(image, "%d", col);
	fscanf(image, "%d", lin);
	fscanf(image, "%d", max);

	pgm_t * pgm = inicializa_pgm("P2", 10, 10, 10);	

	info_pgm(pgm);


	

}
