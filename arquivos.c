#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivos.h"
#include "pgm.h"

#define LINESIZE 1024

void le_entradas_input(FILE ** image, char * input, char ** tipo, int  *col, int *lin, int *max)
{
	*image = fopen(input, "r");

	if (!*image)
	{
		perror("Erro ao abrir arquivo de imagem");
		exit(1);
	}

	//Leitura de arquivos P2

	*tipo = malloc(sizeof(char) * 2 + 2);

	fscanf(*image, "%s", *tipo);

	verifica_comentario(image, tipo);

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

void verifica_comentario(FILE ** image, char ** tipo)
{
	char comentario[LINESIZE + 1];

	fscanf(*image, "%s", comentario);

	if (comentario[0] == '#')
		fgets(comentario, LINESIZE, *image);
	else
		fseek(*image, sizeof(char) * 2, SEEK_SET);
}

void le_entradas(FILE ** image, char * input, char ** tipo_arquivo, int *col, int *lin,int *max)
{
	if (input)
		le_entradas_input(image, input, tipo_arquivo, col, lin, max);
	else
		le_entradas_stdin(tipo_arquivo, col, lin, max);
}

void escreve_saidas_p2_otp(FILE * image_otp, void * void_pgm)
{
	pgm_t * pgm = void_pgm;

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

void escreve_saidas_p5_otp(FILE * image_otp, void * void_pgm)
{
	pgm_t * pgm = void_pgm;

	fprintf(image_otp, "%s\n", pgm->tipo_arquivo);
	fprintf(image_otp, "%d %d\n", pgm->col, pgm->lin);
	fprintf(image_otp, "%d\n", pgm->max);

	for (int i = 0; i < pgm->lin; i++)
		for (int j = 0; j < pgm->col; j++)
			fwrite(&pgm->matriz_pixels[i][j], 1, 1, image_otp);
}

void escreve_saidas_p2_stdout(void * void_pgm)
{
	pgm_t * pgm = void_pgm;

	fprintf(stdout, "%s\n", pgm->tipo_arquivo);
	fprintf(stdout, "%d %d\n", pgm->col, pgm->lin);
	fprintf(stdout, "%d\n", pgm->max);

	for (int i = 0; i < pgm->lin; i++)
	{	
		for (int j = 0; j < pgm->col; j++)
			fprintf(stdout, "%d ", pgm->matriz_pixels[i][j]);
		fprintf(stdout, "\n");
	}

}

void escreve_saidas_p5_stdout(void * void_pgm)
{
	pgm_t * pgm = void_pgm;

	fprintf(stdout, "%s\n", pgm->tipo_arquivo);
	fprintf(stdout, "%d %d\n", pgm->col, pgm->lin);
	fprintf(stdout, "%d\n", pgm->max);

	for (int i = 0; i < pgm->lin; i++)
		for (int j = 0; j < pgm->col; j++)
			fwrite(&pgm->matriz_pixels[i][j], 1, 1, stdout);
}

void escreve_saidas(void * pgm, char * output)
{
	if (output)
	{
		FILE * image_otp;

		image_otp = fopen(output, "w+");

		if (!image_otp)
		{
			perror("Erro ao abrir arquivo de output");
			exit(1);
		}

		if (eh_arquivo_p2( (pgm_t *) pgm) )
			escreve_saidas_p2_otp(image_otp, pgm);

		if (eh_arquivo_p5( (pgm_t *) pgm) )
			escreve_saidas_p5_otp(image_otp, pgm);
	}
	
	else
	{
		if (eh_arquivo_p2( (pgm_t *) pgm) )
			escreve_saidas_p2_stdout(pgm);
		
		if (eh_arquivo_p5( (pgm_t *) pgm) )
			escreve_saidas_p5_stdout(pgm);
	}
	
}
