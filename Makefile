all:
	gcc -c pgm.c -o pgm
	gcc -c arquivos.c -o arquivos
	gcc -c parser.c -o parser
	gcc -g pgmnega.c pgm arquivos parser -o pgmnega
