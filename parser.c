#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "parser.h"

#define MAX_NAME_SIZE 100
#define MAX 100

// Verifica se o argumento existe após a flag
void verifica_arg(int argc, int atual)
{
	if (atual + 1 >= argc)
	{
		fprintf(stderr, "Usage -a N -l N -m N -i input -o output\n");
		exit(1);
	}
}

void parser(int argc, char **argv, char ** input, char ** output, double * angulo, float * limiar, int * mediana)
{
	for (int i = 0; i < argc; i++)
	{
		// Verifica se foi passado input
		if (!strcmp(argv[i], "-i"))
		{
			verifica_arg(argc, i);

			*input = malloc(sizeof(char) * MAX_NAME_SIZE);
			strcpy(*input, argv[i+1]);
		}

		// Verifica se foi passado output
		if (!strcmp(argv[i], "-o"))
		{
			verifica_arg(argc, i);

			*output = malloc(sizeof(char) * MAX_NAME_SIZE);
			strcpy(*output, argv[i+1]);
		}

		// Verifica se foi passado angulo, e o filtro o utiliza
		if (!strcmp(argv[i], "-a") && angulo)
		{
			verifica_arg(argc, i);
			*angulo = atof(argv[i+1]);
		}

		else if (!strcmp(argv[i], "-a") && !angulo)
			fprintf(stderr, "Este filtro não utiliza ângulo, portanto, será ignorado.\n");


		// Verifica se foi passado limiar, e o filtro o utiliza
		if (!strcmp(argv[i], "-l") && limiar)
		{
			verifica_arg(argc, i);
			*limiar = atof(argv[i+1]);
		}

		else if (!strcmp(argv[i], "-l") && !limiar)
			fprintf(stderr, "Este filtro não utiliza limiar, portanto, será ignorado.\n");
		

		//  Verifica se foi passada mediana, e o filtro a utiliza
		if (!strcmp(argv[i], "-m") && mediana)
		{
			verifica_arg(argc, i);
			*mediana = atoi(argv[i+1]);
		}

		else if (!strcmp(argv[i], "-m") && !mediana)
			fprintf(stderr, "Este filtro não utiliza mediana, portanto, será ignorada.\n");

	}

	


}
