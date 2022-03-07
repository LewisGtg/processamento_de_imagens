all:
	gcc -c -Wall pgm.c
	gcc -c -Wall arquivos.c
	gcc -c -Wall parser.c
	gcc -g -Wall pgmnega.c pgm.o arquivos.o parser.o -o pgmnega
	gcc -g -Wall pgmrotacao.c pgm.o arquivos.o parser.o -o pgmrotacao -lm
	gcc -g -Wall pgmlimiar.c pgm.o arquivos.o parser.o -o pgmlimiar
	gcc -g -Wall pgmmedia.c pgm.o arquivos.o parser.o -o pgmmedia
	gcc -g -Wall pgmmediana.c pgm.o arquivos.o parser.o -o pgmmediana
	gcc -g -Wall pgmlbp.c pgm.o arquivos.o parser.o -o pgmlbp

clean:
	rm pgmnega pgmrotacao pgmlimiar pgmmedia pgmmediana pgmlbp
