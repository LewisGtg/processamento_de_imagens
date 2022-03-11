#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pgm.h"
#include "parser.h"
#include "arquivos.h"

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
	int limite; 

	//Numero que elementos que compoe a mascara
	int tam_mascara;

	//Valores dos numeros que compoe a mascara
	int * valores;

	int ** matriz_copia;

	//Propriedades do arquivo pgm
	char * tipo_arquivo;
	int col, lin, max;

	//Faz o parsing das entradas
	parser(argc, argv, &input, &output, NULL, NULL, &mediana);

	//Verifica se o valor passado eh impar
	if (mediana % 2 == 0 || mediana < 3)
	{
		perror("Deve ser passado um valor impar maior que zero para a mediana");
		exit(1);
	}

	//Le as propriedades do arquivo pgm, sem ler a matriz de pixels
	le_propriedades_pgm(&image, input, &tipo_arquivo, &col, &lin, &max);

	//Inicia a struct pgm com as propriedades definidas
	pgm_t * pgm = inicializa_pgm(tipo_arquivo, col, lin, max);	

	//Copia a matriz de pixels para a struct pgm
	le_matriz_pgm(pgm, &image, input);

	//Cria uma copia para a matriz de pixels do pgm
	matriz_copia = inicializa_matriz(col, lin);
	copia_matriz_pgm(pgm, matriz_copia);


	limite = (mediana / 2);
	tam_mascara = mediana * mediana;

	valores = malloc(sizeof(int) * tam_mascara);

	//Aplica o filtro de mediana
	//Inicia e termina em linhas que respeitam o tamanho da mascara
	for (int i = limite; i < pgm->lin - limite; i++)
		//Inicia e termina em colunas que respeitam o tamanho da mascara
		for (int j = limite; j < pgm->col - limite; j++)
		{
			//Passa por todos os vizinhos dentro do limite da mascara, e armazena seus valores em um vetor
			int cont = 0;
			for (int linha = i - limite; linha < mediana + i - limite; linha++)
			   for (int coluna = j - limite; coluna < mediana + j - limite; coluna++)
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
	gera_pgm(pgm, output);
	
	//Fecha arquivo e desaloca estruturas usadas
	free(valores);
	fecha_arquivo(image, input);
	destroi_matriz(matriz_copia);
	destroi_pgm(pgm);
}

int compara(const void *a, const void *b)
{
	return ( *(int*)a - *(int*)b );
}
