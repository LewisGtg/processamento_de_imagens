#ifndef __PGM__
#define __PGM__

typedef struct {
	char tipo_arquivo[2+1];
	int col, lin, max;
	int **matriz_pixels;
} pgm_t;

//Inicia arquivo pgm e retorna seu endereco
pgm_t * inicializa_pgm();

//Desaloca todas as estruturas utilizadas pelo pgm e o proprio pgm
void destroi_pgm(pgm_t * pgm);

//Inicializa uma matriz para armazenar "pixels" do pgm
int **inicializa_matriz(int col, int lin);

//Printa as propriedades do arquivo pgm
void info_pgm(pgm_t * pgm);

//Verifica se o arquivo pgm eh do tipo P2
int eh_arquivo_p2(pgm_t * pgm);

//Verifica se o arquivo pgm eh do tipo P5
int eh_arquivo_p5(pgm_t * pgm);

//Copia a matriz da imagem pgm para a struct pgm
void copia_matriz(pgm_t * pgm, FILE ** file, char * input);

#endif
