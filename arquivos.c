#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivos.h"
#include "pgm.h"

#define LINESIZE 1024

//Abre um arquivo e retorna seu ponteiro
static FILE * abre_arquivo(char * nome_arq, char tipo_saida[2], char tipo[3])
{
	FILE * image;

	//Verifica se foi passado o nome do arquivo como argumento
	if (nome_arq)
	{
		image = fopen(nome_arq, tipo);

		if (!image)
		{
			perror("Erro ao abrir arquivo de imagem");
			exit(1);
		}
	}

	//Retorna um ponteiro para entrada/saida padrao
	else
	{
		if (tipo_saida[0] == 'i')
			image = stdin;
		
		else if (tipo_saida[0] == 'o')
			image = stdout;
	}

	return image;
}

void fecha_arquivo(FILE * image, char * input)
{
	//Verifica se foi passado um arquivo de input
	if (input)
		//Fecha o arquivo passado
		fclose(image);
}

//Verifica se ha comentario apos a definicao do tipo do PGM
void verifica_comentario(FILE ** image)
{
	char comentario[LINESIZE + 1];

	fscanf(*image, "%s", comentario);

	//Verifica se foi lido algum comentario
	if (comentario[0] == '#')
		//"Captura" a linha do comentario, fazendo o ponteiro do arquivo apontar para a proxima linha
		fgets(comentario, LINESIZE, *image);

	else
		//Nao havia comentario, portanto, arruma o ponteiro do arquivo
		fseek(*image, sizeof(char) * 2, SEEK_SET);
}

//Copia o arquivo PGM tipo P2 com filtro aplicado para um arquivo
void escreve_matriz_p2(FILE * image_otp, void * void_pgm)
{
	pgm_t * pgm = void_pgm;

	for (int i = 0; i < pgm->lin; i++)
	{	
		for (int j = 0; j < pgm->col; j++)
			fprintf(image_otp, "%d ", pgm->matriz_pixels[i][j]);
		fprintf(image_otp, "\n");
	}
}

//Copia o arquivo PGM tipo P5 com filtro aplicado para um arquivo 
void escreve_matriz_p5(FILE * image_otp, void * void_pgm)
{
	pgm_t * pgm = void_pgm;

	//Copia a matriz de pixels do PGM para o output
	for (int i = 0; i < pgm->lin; i++)
		for (int j = 0; j < pgm->col; j++)
			fwrite(&pgm->matriz_pixels[i][j], 1, 1, image_otp);
}

//Chama as funcoes de leitura de arquivo por input ou stdin, de acordo com oque foi passado
void le_entradas(FILE ** image, char * input, char ** tipo_arquivo, int *col, int *lin,int *max)
{
	*image = abre_arquivo(input, "i", "r");

	//Aloca memoria para a string que representa o tipo de arquivo
	*tipo_arquivo = malloc(sizeof(char) * 2 + 2);
	fscanf(*image, "%s", *tipo_arquivo);

	//Verifica se ha algum comentario no arquivo pgm
	verifica_comentario(image);

	//Le as demais propriedades do arquivo pgm
	fscanf(*image, "%d", col);
	fscanf(*image, "%d", lin);
	fscanf(*image, "%d", max);
}

//Chama as funcoes de escrita de arquivos por output ou stdout, de acordo com oque foi passado
void escreve_saidas(void * void_pgm, char * output)
{
	FILE * image_otp = abre_arquivo(output, "o", "w+");
	pgm_t * pgm = void_pgm;

	//Copia as propriedades do PGM para o output
	fprintf(image_otp, "%s\n", pgm->tipo_arquivo);
	fprintf(image_otp, "%d %d\n", pgm->col, pgm->lin);
	fprintf(image_otp, "%d\n", pgm->max);

	//Copia a matriz de pixels do PGM para o output
	if (eh_arquivo_p2(pgm))
		escreve_matriz_p2(image_otp, pgm);
	else
		escreve_matriz_p5(image_otp, pgm);
}
