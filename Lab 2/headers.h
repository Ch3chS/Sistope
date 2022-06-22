#ifndef _HEADERS
#define _HEADERS


//--------------------------------------  Estructuras y datos globales  --------------------------------------------------------------

FILE *fp;    //Archivo de donde las hebras leeran (será abierto antes de la creación de estas)

pthread_t threads[]; //lista con las id de las threads creadas

struct nodo{   //Para TDA lista enlazada
    char *dato;
    struct nodo *sig;
};

struct disk{  //Estructura que representa a un disco y es donde se almacenan sus respectivas propiedades
    double media_r;
    double media_i;
    double potencia;
    double ruido;
    int visibilidades_totales;  //Este es un contador de visibilidades básicamente (Para la división al final)
};

struct disk discos[];           //Arreglo donde se almacenan los distintos discos con sus propiedades

int endflag = 0;     //Bandera que permitirá avisar cuando el archivo llegue a su fin a las demas hebras




//-------------------------------------------------  Mutex  -----------------------------------------------------------------

pthread_mutex_t Leer;           //Semaforo encargado de que solo una hebra lea del archivo a la vez

pthread_mutex_t Edit_disk[];    //Semaforo encargado de que solo una hebra escriba en un disco a la vez




//---------------------------------------------  Funciones main  ------------------------------------------------------------

void file_open(char *file_name);

void threads_creation(int h, int args[3]);

void wait_threads(int h);

void average_division(int n);

void write_file(char *file_name, int n, int b);




//---------------------------------------------  Funciones hebra ------------------------------------------------------------

void thread(int args[3]);




//---------------------------------------------  Funciones TDA------------------------------------------------------------

struct nodo *agregar_nodo(char *nuevo_dato, struct nodo *lista);

char *leer_nodo(struct nodo *lista);

struct nodo *borrar_inicio(struct nodo *lista);




//---------------------------------------------------------------------------------------------------------------------------

#include "Functions/main_functions.c"
#include "Functions/thread_functions.c"
#include "TDAs/Lista_enlazada.c"

#endif