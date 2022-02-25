typedef struct {
	char tipo_arquivo[2+1];
	int col, lin, max;
	int **matriz_pixels;
} pgm_t;

//Aloca memoria para o arquivo pgm
pgm_t * inicializa_pgm();

int **inicializa_matriz(int col, int lin);

void info_pgm(pgm_t * pgm);

int eh_arquivo_p2(pgm_t * pgm);

int eh_arquivo_p5(pgm_t * pgm);

void copia_matriz(pgm_t * pgm, FILE ** file, char * input);

void copia_matriz_p2_input(pgm_t * pgm, FILE ** file);

void copia_matriz_p2_stdin(pgm_t * pgm);

void copia_matriz_p5_input(pgm_t * pgm, FILE ** file);

void copia_matriz_p5_stdin(pgm_t * pgm);
