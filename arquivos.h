void le_entradas_input(FILE ** image, char * input, char ** tipo, int  *col, int *lin, int *max);

void le_entradas_stdin(char ** tipo, int  *col, int *lin, int *max);

void le_entradas(FILE ** image, char * input, char ** tipo_arquivo, int *col, int *lin,int *max);

void escreve_saidas(void * pgm, char * output);

void escreve_saidas_p2_otp(FILE * image_otp, void * pgm);

void escreve_saidas_p5_otp(FILE * image_otp, void * pgm);

void escreve_saidas_p2_stdout(void * pgm);

void escreve_saidas_p5_stdout(void * pgm);
