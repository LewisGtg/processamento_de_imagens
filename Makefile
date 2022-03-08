OBJETOS = pgm.o arquivos.o parser.o
EXECUTAVEIS = pgmnega pgmrotacao pgmlimiar pgmmedia pgmmediana pgmlbp

all: $(EXECUTAVEIS)

# Arquivos executaveis
pgmnega: $(OBJETOS) pgmnega.c
	gcc -g -Wall pgmnega.c pgm.o arquivos.o parser.o -o pgmnega

pgmrotacao: $(OBJETOS) pgmrotacao.c
	gcc -g -Wall pgmrotacao.c pgm.o arquivos.o parser.o -o pgmrotacao -lm

pgmlimiar: $(OBJETOS) pgmlimiar.c
	gcc -g -Wall pgmlimiar.c pgm.o arquivos.o parser.o -o pgmlimiar

pgmmedia: $(OBJETOS) pgmmedia.c
	gcc -g -Wall pgmmedia.c pgm.o arquivos.o parser.o -o pgmmedia

pgmmediana: $(OBJETOS) pgmmediana.c
	gcc -g -Wall pgmmediana.c pgm.o arquivos.o parser.o -o pgmmediana

pgmlbp: $(OBJETOS) pgmlbp.c
	gcc -g -Wall pgmlbp.c pgm.o arquivos.o parser.o -o pgmlbp


# Arquivos objeto
pgm.o: pgm.c pgm.h
	gcc -c -Wall pgm.c

arquivos.o: arquivos.c arquivos.h
	gcc -c -Wall arquivos.c

parser.o: parser.c parser.h
	gcc -c -Wall parser.c

# Limpeza de arquivos
clean:
	-rm *.o

purge: clean
	-rm pgmnega pgmrotacao pgmlimiar pgmmedia pgmmediana pgmlbp
