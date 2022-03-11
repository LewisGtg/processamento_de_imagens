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

	int ** matriz_copia;

	//Propriedades do arquivo pgm
	char * tipo_arquivo;
	int col, lin, max;

	//Faz o parsing das entradas
	parser(argc, argv, &input, &output, NULL, NULL, NULL);

	//Le as propriedades do arquivo pgm, sem ler a matriz de pixels
	le_propriedades_pgm(&image, input, &tipo_arquivo, &col, &lin, &max);

	//Inicia a struct pgm com as propriedades definidas
	pgm_t * pgm = inicializa_pgm(tipo_arquivo, col, lin, max);	

	//Copia a matriz de pixels para a struct pgm
	le_matriz_pgm(pgm, &image, input);
	
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
					// Verifica se o pixel atual eh o pixel do meio
					if (linha == i && coluna == j)
						continue;

					// Faz as somas do lbp
					if (matriz_copia[linha][coluna] >= matriz_copia[i][j])
						soma += exp;		
					exp *= 2;
				}
			}
			pgm->matriz_pixels[i][j] = soma;
		}
	//Copia o pgm com o filtro aplicado para algum arquivo de saida
	gera_pgm(pgm, output);
	
	//Fecha arquivo e desaloca estruturas usadas
	fecha_arquivo(image, input);
	destroi_matriz(matriz_copia);
	destroi_pgm(pgm);
}

