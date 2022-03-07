#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arquivos.h"
#include "pgm.h"

#define LINESIZE 1024

//Verifica se ha comentario apos a definicao do tipo do PGM
static void verifica_comentario(FILE ** image)
{
	char comentario[LINESIZE + 1];

	//Verifica se foi passado o arquivo pgm como argumento
	if (image)
	{
		fscanf(*image, "%s", comentario);

		//Verifica se foi lido algum comentario
		if (comentario[0] == '#')
			//"Captura" a linha do comentario, fazendo o ponteiro do arquivo apontar para a proxima linha
			fgets(comentario, LINESIZE, *image);

		else
			//Nao havia comentario, portanto, arruma o ponteiro do arquivo
			fseek(*image, sizeof(char) * 2, SEEK_SET);
	}

	//Verifica comentario de arquivo passado pelo stdin
	else
	{
		fscanf(stdin, "%s", comentario);

		//Verifica se foi lido algum comentario
		if (comentario[0] == '#')
			//"Captura" a linha do comentario, fazendo o ponteiro do arquivo apontar para a proxima linha
			fgets(comentario, LINESIZE, stdin);

		else
			//Nao havia comentario, portanto, arruma o ponteiro do arquivo
			fseek(stdin, sizeof(char) * 2, SEEK_SET);
	}
}

//Le o arquivo PGM passado por argumento
static void le_entradas_input(FILE ** image, char * input, char ** tipo, int  *col, int *lin, int *max)
{
	//Abre o arquivo passado por argumento, e testa se deu certo
	*image = fopen(input, "r");

	if (!*image)
	{
		perror("Erro ao abrir arquivo de imagem");
		exit(1);
	}

	//Aloca memoria para a string que representa o tipo de arquivo
	*tipo = malloc(sizeof(char) * 2 + 2);
	fscanf(*image, "%s", *tipo);

	//Verifica se ha algum comentario no arquivo pgm
	verifica_comentario(image);

	//Le as demais propriedades do arquivo pgm
	fscanf(*image, "%d", col);
	fscanf(*image, "%d", lin);
	fscanf(*image, "%d", max);
}

//Le arquivo PGM passado pelo stdin
static void le_entradas_stdin(char ** tipo, int  *col, int *lin, int *max)
{
	//Aloca memoria para a string que representa o tipo de arquivo
	*tipo = malloc(sizeof(char) * 2 + 2);
	fscanf(stdin, "%s", *tipo);
	
	//Verifica se ha algum comentario no arquivo pgm
	verifica_comentario(NULL);

	//Le as demais propriedades do arquivo pgm
	fscanf(stdin, "%d", col);
	fscanf(stdin, "%d", lin);
	fscanf(stdin, "%d", max);
}

//Copia o arquivo PGM tipo P2 com filtro aplicado para um arquivo de saida passado como argumento
static void escreve_saidas_p2_otp(FILE * image_otp, void * void_pgm)
{
	//"Casting" do pgm passado como void
	pgm_t * pgm = void_pgm;

	//Copia as propriedades do PGM para o output
	fprintf(image_otp, "%s\n", pgm->tipo_arquivo);
	fprintf(image_otp, "%d %d\n", pgm->col, pgm->lin);
	fprintf(image_otp, "%d\n", pgm->max);

	//Copia a matriz de pixels do PGM para o output
	for (int i = 0; i < pgm->lin; i++)
	{	
		for (int j = 0; j < pgm->col; j++)
			fprintf(image_otp, "%d ", pgm->matriz_pixels[i][j]);
		fprintf(image_otp, "\n");
	}
}

//Copia o arquivo PGM tipo P5 com filtro aplicado para um arquivo de saida passado como argumento
static void escreve_saidas_p5_otp(FILE * image_otp, void * void_pgm)
{
	//"Casting" do pgm passado como void
	pgm_t * pgm = void_pgm;

	//Copia as propriedades do PGM para o output
	fprintf(image_otp, "%s\n", pgm->tipo_arquivo);
	fprintf(image_otp, "%d %d\n", pgm->col, pgm->lin);
	fprintf(image_otp, "%d\n", pgm->max);

	//Copia a matriz de pixels do PGM para o output
	for (int i = 0; i < pgm->lin; i++)
		for (int j = 0; j < pgm->col; j++)
			fwrite(&pgm->matriz_pixels[i][j], 1, 1, image_otp);
}

//Copia o arquivo PGM tipo P2 com filtro aplicado para o stdout
static void escreve_saidas_p2_stdout(void * void_pgm)
{
	//"Casting" do pgm passado como void
	pgm_t * pgm = void_pgm;

	//Copia as propriedades do PGM para o stdout
	fprintf(stdout, "%s\n", pgm->tipo_arquivo);
	fprintf(stdout, "%d %d\n", pgm->col, pgm->lin);
	fprintf(stdout, "%d\n", pgm->max);

	//Copia a matriz de pixels do PGM para o stdout
	for (int i = 0; i < pgm->lin; i++)
	{	
		for (int j = 0; j < pgm->col; j++)
			fprintf(stdout, "%d ", pgm->matriz_pixels[i][j]);
		fprintf(stdout, "\n");
	}

}

//Copia o arquivo PGM tipo P5 com filtro aplicado para o stdout
static void escreve_saidas_p5_stdout(void * void_pgm)
{
	//"Casting" do pgm passado como void
	pgm_t * pgm = void_pgm;

	//Copia as propriedades do PGM para o stdout
	fprintf(stdout, "%s\n", pgm->tipo_arquivo);
	fprintf(stdout, "%d %d\n", pgm->col, pgm->lin);
	fprintf(stdout, "%d\n", pgm->max);

	//Copia a matriz de pixels do PGM para o stdout
	for (int i = 0; i < pgm->lin; i++)
		for (int j = 0; j < pgm->col; j++)
			fwrite(&pgm->matriz_pixels[i][j], 1, 1, stdout);
}

//Chama as funcoes de leitura de arquivo por input ou stdin, de acordo com oque foi passado
void le_entradas(FILE ** image, char * input, char ** tipo_arquivo, int *col, int *lin,int *max)
{
	if (input)
		le_entradas_input(image, input, tipo_arquivo, col, lin, max);
	else
		le_entradas_stdin(tipo_arquivo, col, lin, max);
}

//Chama as funcoes de escrita de arquivos por output ou stdout, de acordo com oque foi passado
void escreve_saidas(void * pgm, char * output)
{
	//Verifica se foi passado arquivo de output como argumento
	if (output)
	{
		FILE * image_otp;

		//Abre um arquivo para escrita e testa se deu certo
		image_otp = fopen(output, "w+");

		if (!image_otp)
		{
			perror("Erro ao abrir arquivo de output");
			exit(1);
		}

		//Escreve no arquivo de saida de acordo com o tipo do PGM
		if (eh_arquivo_p2( (pgm_t *) pgm) )
			escreve_saidas_p2_otp(image_otp, pgm);

		if (eh_arquivo_p5( (pgm_t *) pgm) )
			escreve_saidas_p5_otp(image_otp, pgm);

		fecha_arquivo(image_otp, output);
	}
	
	//Escreve no arquivo passado no stdout
	else
	{
		//Escreve no arquivo de sauda de acordo com o tipo do PGM
		if (eh_arquivo_p2( (pgm_t *) pgm) )
			escreve_saidas_p2_stdout(pgm);
		
		if (eh_arquivo_p5( (pgm_t *) pgm) )
			escreve_saidas_p5_stdout(pgm);
	}
}

void fecha_arquivo(FILE * image, char * input)
{
	//Verifica se foi passado um arquivo de input
	if (input)
		//Fecha o arquivo passado
		fclose(image);
}
