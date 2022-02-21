all:
	gcc -c pgm.c -o pgm
	gcc -g pgmnega.c pgm -o pgmnega
