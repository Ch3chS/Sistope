#ifndef _HEADERS
#define _HEADERS


//--------------------------------------  Estructuras y datos globales  --------------------------------------------------------------

FILE *fp;    //Archivo de donde las hebras leeran (será abierto antes de la creación de estas)

pthread_t threads[]; //lista con las id de las threads creadas



struct disk{
    double media_r;
    double media_i;
    double potencia;
    double ruido;
    int visibilidades_totales;
};

struct leido{
    int largo_lista;
    char *lineas[];
};

struct disk discos[];

int endflag = 0;   //Esta bandera servirá cuando una de las hebras llegue al final del archivo



//---------------------------------------------  Funciones main  ------------------------------------------------------------

void file_open(char *file_name);

void threads_creation(int h, int args[3]);

void wait_threads(int h);

void average_division(int n);

void write_file(char *file_name, int n, int b);

//---------------------------------------------  Funciones hebra ------------------------------------------------------------

void thread(int args[3]);


//---------------------------------------------------------------------------------------------------------------------------

#include "Functions/main_functions.c"
#include "Functions/thread_functions.c"

#endif