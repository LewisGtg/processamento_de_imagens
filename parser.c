#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"

#define MAX_NAME_SIZE 100

void define_io(int argc, char **argv, char ** input, char ** output)
{
	int option;

	while ((option = getopt(argc, argv, "i:o:")) != -1)
	{
		switch(option)
		{
		 	case 'i':
				*input = malloc(sizeof(char) * MAX_NAME_SIZE);
				strcpy(*input, optarg);
				break;

			case 'o':
				*output = malloc(sizeof(char) * MAX_NAME_SIZE);
				strcpy(*output, optarg);
				break;

			default:
				fprintf(stderr, "Usage -i input -o output\n");
				exit(1);
		}

	}


}

