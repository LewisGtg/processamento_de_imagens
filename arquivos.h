#include "pgm.h"

#ifndef __ARQ__
#define __ARQ__

//Le as propriedades do arquivo PGM
void le_propriedades_pgm(FILE ** image, char * input, char ** tipo_arquivo, int *col, int *lin,int *max);

//Copia a matriz da imagem pgm para a struct pgm
void le_matriz_pgm(pgm_t * pgm, FILE ** file, char * input);

//Cria um arquivo contendo o PGM com o filtro aplicado
void gera_pgm(pgm_t * pgm, char * output);

//Fecha o arquivo passado como input, caso tenha sido passado
void fecha_arquivo(FILE * image, char * input);

#endif
