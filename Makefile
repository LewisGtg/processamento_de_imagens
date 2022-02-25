all:
	gcc -c -Wall pgm.c -o pgm
	gcc -c -Wall arquivos.c -o arquivos
	gcc -c -Wall parser.c -o parser
	gcc -g -Wall pgmnega.c pgm arquivos parser -o pgmnega
	gcc -g -Wall pgmrotacao.c pgm arquivos parser -o pgmrotacao
	gcc -g -Wall pgmlimiar.c pgm arquivos parser -o pgmlimiar
	gcc -g -Wall pgmmedia.c pgm arquivos parser -o pgmmedia
	gcc -g -Wall pgmmediana.c pgm arquivos parser -o pgmmediana
	gcc -g -Wall pgmlbp.c pgm arquivos parser -o pgmlbp

clean:
	rm pgmnega pgmrotacao pgmlimiar pgmmedia pgmmediana pgmlbp
