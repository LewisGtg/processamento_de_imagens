#ifndef __ARQ__
#define __ARQ__

//Le as propriedades do arquivo PGM
void le_entradas(FILE ** image, char * input, char ** tipo_arquivo, int *col, int *lin,int *max);

//Cria um arquivo contendo o PGM com o filtro aplicado
void escreve_saidas(void * pgm, char * output);

//Fecha o arquivo passado como input, caso tenha sido passado
void fecha_arquivo(FILE * image, char * input);

#endif
