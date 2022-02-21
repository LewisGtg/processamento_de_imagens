all:
	gcc -c pgm.c -o pgm
	gcc -c arquivo.c -o arquivo
	gcc -g pgmnega.c pgm arquivo -o pgmnega
