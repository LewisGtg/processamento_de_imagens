#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"

#define MAX_NAME_SIZE 100
#define MAX 100

void define_io(int argc, char **argv, char ** input, char ** output, float * angulo, float * limiar, float * media)
{
	int option;
	char * angulo_char = NULL;
	char * limiar_char = NULL;
	char * media_char = NULL;

	while ((option = getopt(argc, argv, "i:o:a:l:m:")) != -1)
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

			case 'a':
				if (angulo)
				{
					angulo_char = optarg;
					*angulo = atof(angulo_char);
				}
				
				break;	
			
			case 'l':
				if (limiar)
				{
					limiar_char = optarg;
					*limiar = atof(limiar_char);
				}

				break;

			case 'm':
				if (media)
				{
					media_char = optarg;
					*media = atof(media_char);
				}

				break;

			default:
				fprintf(stderr, "Usage -a N -l N -m N -i input -o output\n");
				exit(1);
		}

	}


}

