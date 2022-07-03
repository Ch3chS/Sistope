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

pthread_mutex_t Arreglo_edit_disk;   //El arreglo de semaforos para los discos me estaba dando problemas asi que le puse un semaforo propio




//---------------------------------------------  Funciones main  ------------------------------------------------------------

//Entrada: Nombre de un archivo como cadena de caracteres
//Salida: No posee
//Abre el archivo sin escribir nada en el, y avisa en caso de un error al abrir el mismo
void file_open(char *file_name);

//Entrada: Número de anillos
//Salida: No posee
//Inicializa los discos con sus valores en 0
void inicializar_anillos(int n);

//Entrada: Entero con el número de discos
//Salida: No posee
//Inicializa los diferentes semaforos a usar a lo largo del programa para asi evitar problemas de concurrencia
void semaphores_init(int n);

//Entrada: Un entero con el número de hebras y un arreglo de enteros con los argumentos a usar por estas
//Salida: No posee
//Crea las hebras pasandoles el argumento solicitado (La id de las hebras queda guardada en un arreglo para la siguiente función)
void threads_creation(int h, int args[3]);

//Entrada: El número de hebras (También hace uso del arreglo rellenado en la función previa)
//Salida: No posee
//Espera a que finalicen todas las hebras una por una
void wait_threads(int h);

//Entrada: Entero con el número de discos
//Salida: No posee
//Divide lo acumulado en la media real e imaginaria de cada disco (A menos que estos no tengan ninguna visibilidad dentro)
void average_division(int n);

//Entrada: El nombre de un archivo a modo de cadena de caracteres, un entero con el número de discos y una flag que nos dice si debemos imprimir o no por pantalla
//Salida: No posee
//Escribe los datos obtenidos en un archivo con un nombre dado, y, en caso de pedirse por consola con "-b" también se debe imprimir lo obtenido en la misma
void write_file(char *file_name, int n, int b);




//---------------------------------------------  Funciones hebra ------------------------------------------------------------

//Entrada: Entero con el número de lineas a leer del archivo por vez que se ejecuta la función
//Salida: Una lista enlazada
//Lee linea por linea un chunk y agrega cada una de estas lineas en una lista enlazada previamente creada (La lista va al revés de como se lee el archivo (El orden no altera el resultado en este caso))
struct nodo *CS_readChunk(int chunk);

//Entrada: Número de anillo, parte real de la visibilidad, parte imaginaria y ruido (los ultimos 3 como flotantes dobles y el primero como entero
//Salida: No posee
//Actualiza los valores dentro del anillo con los valores obtenidos de la linea correspondiente a este
void CS_actualizarAnillo(int anillo, double real, double imaginario, double ruido);

//Entrada: Arreglo de flotantes dobles con los datos de la linea, el número de discos y el ancho de cada uno 
//Salida: Un entero que indica si termino correctamente
//A partir de los datos obtenidos se ve en que disco se encuentra la visibilidad, posteriormente usan semaforos y se entra a dicho disco de forma segura
void seleccionar_anillo(double datos[5], int n_discos, int radio);

//Entrada: Cadena de caractertes con la linea a procesar, el número de discos y el ancho de cada disco (Los ultimos 2 para pasarlos a la siguiente función)
//Salida: No posee
//Procesa la cadena de caracteres para convertir sus valores contenidos en flotantes dobles, con los valores ya convertidos se llama a otra función para que dirija estos valores a un disco en especifico
void procesar_linea(char *linea, int n , int d);

//Entrada: Una lista enlazada con un chunk de datos (También recibe el número de discos y el ancho de estos pero es solo para pasarlos a otra función)
//Salida: No posee
//Recorre la lista enlazada para que se procese el contenido linea a linea (El proceso de la linea es realizado por otra función)
void procesar_chunk(struct nodo *L, int n, int d);

//Entrada: Argumentos a usar por las hebras (Número de discos, Ancho de discos, Chunk)
//Salida: No posee
//Función principal de las hebras donde no hay más que una estructura bastante general condicionada al fin del archivo y llamados a funciones más especificas
void thread(int args[3]);




//---------------------------------------------  Funciones TDA------------------------------------------------------------

//Entrada: una cadena de caracteres
//Salida: Un nodo para una lista simplemente enlazada
//Crea un nodo con su respectivo dato y lo retorna para su uso
struct nodo *crear_nodo(char *dato);

//Entrada: Una cadena de caracteres y una lista enlazada
//Salida: Una lista enlazada
//Crea un nodo con el dato haciendo uso de la función anterior y lo agrega al inicio de la lista
struct nodo *agregar_nodo(char *nuevo_dato, struct nodo *lista);

//Entrada: Una lista enlazada
//Salida: Una lista enlazada
//Libera el primer nodo de la lista sin afectar al resto de esta
struct nodo *borrar_inicio(struct nodo *lista);

//Entrada: Una lista enlazada
//Salida: Una cadena de caracteres
//Recibe una lista y retorna el dato del primer nodo
char *leer_nodo(struct nodo *lista);




//---------------------------------------------------------------------------------------------------------------------------

#include "Functions/main_functions.c"
#include "Functions/thread_functions.c"
#include "TDAs/Lista_enlazada.c"

#endif