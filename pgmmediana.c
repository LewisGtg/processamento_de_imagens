#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pgm.h"
#include "parser.h"
#include "arquivos.h"

//Copia a matriz de pixels do pgm para uma outra matriz qualquer
void copia_matriz_pgm(pgm_t * pgm, int ** matriz);

//Funcao utilizada pelo qsort para ordenar um vetor de inteiros
int compara(const void *a, const void *b);

int main(int argc, char **argv)
{
	FILE * image;

	char * input = NULL;
	char * output = NULL;
	
	//Valor que define o tamanho da mascara 
	int mediana = 3;

	//Valor da mediana dos valores que compoe a mascara
	int valor_mediana;

	//Define a distancia do pixel ate a borda da mascara
	int boundary_limit; 

	//Numero que elementos que compoe a mascara
	int tam_mascara;

	//Valores dos numeros que compoe a mascara
	int * valores;

	int ** matriz_copia;

	//Propriedades do arquivo pgm
	char * tipo_arquivo;
	int col, lin, max;

	//Faz o parsing das entradas
	define_io(argc, argv, &input, &output, NULL, NULL, &mediana);

	//Verifica se o valor passado eh impar
	if ((int)mediana % 2 == 0)
	{
		perror("Deve ser passado um valor impar para a mediana");
		exit(1);
	}

	//Le as propriedades do arquivo pgm, sem ler a matriz de pixels
	le_entradas(&image, input, &tipo_arquivo, &col, &lin, &max);

	//Inicia a struct pgm com as propriedades definidas
	pgm_t * pgm = inicializa_pgm(tipo_arquivo, col, lin, max);	

	//Copia a matriz de pixels para a struct pgm
	copia_matriz(pgm, &image, input);

	//Cria uma copia para a matriz de pixels do pgm
	matriz_copia = inicializa_matriz(col, lin);
	copia_matriz_pgm(pgm, matriz_copia);


	boundary_limit = (mediana / 2);
	tam_mascara = mediana * mediana;

	valores = malloc(sizeof(int) * tam_mascara);

	//Aplica o filtro de mediana
	//Inicia e termina em linhas que respeitam o tamanho da mascara
	for (int i = boundary_limit; i < pgm->lin - boundary_limit; i++)
		//Inicia e termina em colunas que respeitam o tamanho da mascara
		for (int j = boundary_limit; j < pgm->col - boundary_limit; j++)
		{
			//Passa por todos os vizinhos dentro do limite da mascara, e armazena seus valores em um vetor
			int cont = 0;
			for (int linha = i - boundary_limit; linha < mediana + i - boundary_limit; linha++)
			   for (int coluna = j - boundary_limit; coluna < mediana + j - boundary_limit; coluna++)
			   {
					valores[cont] = matriz_copia[linha][coluna];
					cont++;
			   } 

			//Ordena os valores encontrados
			qsort(valores, tam_mascara, sizeof(int), compara);

			//Seleciona o valor da mediana dos valores, e troca o valor do pixel atual pela mediana
			valor_mediana = valores[(tam_mascara-1) / 2];
			pgm->matriz_pixels[i][j] = valor_mediana;
		}

	//Copia o pgm com o filtro aplicado para algum arquivo de saida
	escreve_saidas(pgm, output);
}

void copia_matriz_pgm(pgm_t * pgm, int ** matriz)
{
	for (int i = 0; i < pgm->lin; i++)
		for (int j = 0; j < pgm->col; j++)
			matriz[i][j] = pgm->matriz_pixels[i][j];
}

int compara(const void *a, const void *b)
{
	return ( *(int*)a - *(int*)b );
}
