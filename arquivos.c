#include <stdio.h>
#include <stdlib.h>
#include "arquivos.h"

void le_entradas_input(void ** image, char * input, char ** tipo, int  *col, int *lin, int *max)
{
	*image = fopen(input, "r");

	if (!(FILE *)*image)
	{
		perror("Erro ao abrir arquivo de imagem");
		exit(1);
	}

	//Leitura de arquivos P2

	*tipo = malloc(sizeof(char) * 2 + 2);

	fscanf(*image, "%s", *tipo);
	fscanf(*image, "%d", col);
	fscanf(*image, "%d", lin);
	fscanf(*image, "%d", max);
}

void le_entradas_stdin(char ** tipo, int  *col, int *lin, int *max)
{
	*tipo = malloc(sizeof(char) * 2 + 2);

	fscanf(stdin, "%s", *tipo);
	fscanf(stdin, "%d", col);
	fscanf(stdin, "%d", lin);
	fscanf(stdin, "%d", max);

}


void le_entradas(void ** image, char * input, char * tipo_arquivo, int *col, int *lin,int *max)
{
	if (input)
		le_entradas_input(image, input, &tipo_arquivo, col, lin, max);
	else
		le_entradas_stdin(&tipo_arquivo, col, lin, max);
}
