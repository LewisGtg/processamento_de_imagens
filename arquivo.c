#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "arquivo.h"

void define_io(int argc, char **argv, char * input, char * output)
{
	int option;

	while ((option = getopt(argc, argv, "i:o:")) != -1)
	{
		switch(option)
		{
		 	case 'i':
				input = malloc(sizeof(char) * 100);
				printf("%p\n", input);
				printf("%s\n", optarg);
				break;

			case 'o':
				output = optarg;
				break;

			default:
				fprintf(stderr, "Usage -i input -o output\n");
				exit(1);
		}

	}


}
