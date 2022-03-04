#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pgm.h"
#include "parser.h"
#include "arquivos.h"

//Copia a matriz de pixels do pgm para uma outra matriz qualquer
void copia_matriz_pgm(pgm_t * pgm, int ** matriz);

int main(int argc, char **argv)
{
	FILE * image;

	char * input = NULL;
	char * output = NULL;

	int ** matriz_copia;

	//Propriedades do arquivo pgm
	char * tipo_arquivo;
	int col, lin, max;

	//Faz o parsing das entradas
	define_io(argc, argv, &input, &output, NULL, NULL, NULL);

	//Le as propriedades do arquivo pgm, sem ler a matriz de pixels
	le_entradas(&image, input, &tipo_arquivo, &col, &lin, &max);

	//Inicia a struct pgm com as propriedades definidas
	pgm_t * pgm = inicializa_pgm(tipo_arquivo, col, lin, max);	

	//Copia a matriz de pixels para a struct pgm
	copia_matriz(pgm, &image, input);
	
	//Cria uma copia para a matriz de pixels do pgm
	matriz_copia = inicializa_matriz(col, lin);
	copia_matriz_pgm(pgm, matriz_copia);

	//Aplica o filtro LBP
	//Passa por todos os pixels que nao estejam na borda
	for (int i = 1; i < pgm->lin - 1; i++)
		for (int j = 1; j < pgm->col - 1; j++)
		{
			int soma = 0;
			int exp = 1;
	
			//Passa por todos os vizinhos do pixel atual
			for (int linha = i - 1; linha < i + 2; linha ++)
			{
				for (int coluna = j - 1; coluna < j + 2; coluna++)
				{
					if (linha == i && coluna == j)
						continue;

					if (matriz_copia[linha][coluna] >= matriz_copia[i][j])
						soma += exp;		
					exp *= 2;
				}
			}
			pgm->matriz_pixels[i][j] = soma;
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
