#include "../headers.h"


//------------------------------------------  Secciones críticas  ------------------------------------------------------

//Entrada: Entero con el número de lineas a leer del archivo por vez que se ejecuta la función
//Salida: Una lista enlazada
//Lee linea por linea un chunk y agrega cada una de estas lineas en una lista enlazada previamente creada (La lista va al revés de como se lee el archivo (El orden no altera el resultado en este caso))
struct nodo *CS_readChunk(int chunk){
    char *cadena = NULL;   //Tanto la cadena a guardar como el número de bytes inician en nulo y 0 para que la misma getline asigne el espacio
    size_t bytes = 0;
    struct nodo *aux = 0;      //Definición de la lista enlazada
 
    for(int i = 0; i < chunk; i++){
        if(getline(&cadena, &bytes, fp) == -1){   //Si llegamos al final del documento
            endflag = 1;                          //Le avisamos a las demas hebras que el archivo ha terminado
            break;                                //Salimos del bucle para no continuar intentando leer
        }
        aux = agregar_nodo(cadena, aux);   //Agregamos la linea a la lista enlazada
    }

    return aux;    //Una vez se leyeron y procesaron todas las lineas termina la función
}


//Entrada: Número de anillo, parte real de la visibilidad, parte imaginaria y ruido (los ultimos 3 como flotantes dobles y el primero como entero
//Salida: No posee
//Actualiza los valores dentro del anillo con los valores obtenidos de la linea correspondiente a este
void CS_actualizarAnillo(int anillo, double real, double imaginario, double ruido){

    printf("anillo %d\n", anillo);

    double potencia = sqrt(pow(real, 2) + pow(imaginario, 2));

    discos[anillo].media_r += real;

    discos[anillo].media_i += imaginario;

    discos[anillo].potencia += potencia;

    discos[anillo].ruido += ruido;

    discos[anillo].visibilidades_totales += 1;  //Ya que agregamos una visibilidad hay que actualizar el contador para la división final
}




//----------------------------------------------  Funciones  -----------------------------------------------------------


//Entrada: Arreglo de flotantes dobles con los datos de la linea, el número de discos y el ancho de cada uno 
//Salida: Un entero que indica si termino correctamente
//A partir de los datos obtenidos se ve en que disco se encuentra la visibilidad, posteriormente usan semaforos y se entra a dicho disco de forma segura
int seleccionar_anillo(double datos[5], int n_discos, int radio){
    
    int distancia = sqrt(pow(datos[0], 2) + pow(datos[1], 2));       //Modulo (raiz de la suma de los cuadrados de u y v)
    int i = 1;  //La i será un iterador para ubicarnos entre los radios de los distintos discos y asi saber en que disco estamos ubicados

    while(i < n_discos){ 
        if((((i-1)*radio) <= distancia) && (distancia < (i*radio))){  //Se verifica la pertenencia a un disco
            //Numero de disco dado por i
            
            pthread_mutex_lock(&Edit_disk[i]);
            CS_actualizarAnillo(i, datos[2], datos[3], datos[4]);        //---------------ENTRADA AL DISCO i-----------//
            pthread_mutex_unlock(&Edit_disk[i]);

            return 0; //Si ya se encontro el disco y se mandaron los datos retornamos para continuar
        }
        i++;   // Si no entra en el rango de este disco pasamos al siguiente
    }

    //Si salimos del bucle es porque la distancia supera el limite del penultimo disco, es decir pertenece a la capa exterior
    //Entonces el numero de disco dado por n_discos (El ultimo disco)
            pthread_mutex_lock(&Edit_disk[n_discos]);
            CS_actualizarAnillo(n_discos, datos[2], datos[3], datos[4]); //---------------ENTRADA AL DISCO n_discos-----------//
            pthread_mutex_unlock(&Edit_disk[n_discos]);


    return 0;
}


//Entrada: Cadena de caractertes con la linea a procesar, el número de discos y el ancho de cada disco (Los ultimos 2 para pasarlos a la siguiente función)
//Salida: No posee
//Procesa la cadena de caracteres para convertir sus valores contenidos en flotantes dobles, con los valores ya convertidos se llama a otra función para que dirija estos valores a un disco en especifico
void procesar_linea(char *linea, int n , int d){
    
    int i=0, j=0, k=0;  //iteradores
    char temp[100];  //Aqui se iran guardando los caracteres entre las comas y los saltos de linea para posteriormente transformarlos a flotante
    double valores[5] = {0,0,0,0,0};  //Aqui es donde se guardaran los datos finales de la linea {u, v, V(u,v).r, V(u,v).i, V(u,v).w}

    while(linea[i] != '\n'){  //Primero vamos caracter por caracter hasta el final
        
        while(linea[i] != ','){      //En caso de no ser una coma
            temp[j] = linea[i];      //Se almacena el caracter en una lista temporal
            temp[j+1] = '\0';        //Y al siguiente se le pone \0 para decir que termino la cadena (no estoy seguro si es necesario pero lo hago por si acaso)
            if(linea[i+1] == '\n'){  //si el proximo elemento es un salto de linea (el final) pues salimos del ciclo más interno (lo pongo aqui y no en el while para que no nos saltemos el ultimo caracter)
                break;
            }
            
            j++;   //Aumentamos tanto i como j para el siguiente caracter
            i++;
        }
        
        if(linea[i] == ',' || '\n'){   //Cuando hay una coma o salto de linea y no es por el ciclo de antes entonces ya se obtuvo un termino y esta en la lista temporal
            valores[k] = atof(temp);   //Este termino lo convertimos de string a tipo flotante y lo almacenamos en la lista de valores
            k++;
            j=0;  //Volvemos al inicio de la lista temporal para buscar el siguiente valor
        }
        i++;
    }
    
    seleccionar_anillo(valores, n, d);
    return;
}


//Entrada: Una lista enlazada con un chunk de datos (También recibe el número de discos y el ancho de estos pero es solo para pasarlos a otra función)
//Salida: No posee
//Recorre la lista enlazada para que se procese el contenido linea a linea (El proceso de la linea es realizado por otra función)
void procesar_chunk(struct nodo *L, int n, int d){
    while(L != NULL){               //Recorremos el chunk con las respectivas lineas
        if(leer_nodo(L) != NULL){
            procesar_linea(leer_nodo(L), d, n);    //Procesamos la linea
            L = L->sig;
            //L = borrar_inicio(L);                //Y la borramos de la estructura para pasar a la siguiente
        }else return;
    }
}


//Entrada: Argumentos a usar por las hebras (Número de discos, Ancho de discos, Chunk)
//Salida: No posee
//Función principal de las hebras donde no hay más que una estructura bastante general condicionada al fin del archivo y llamados a funciones más especificas
void thread(int args[3]){ //int N_discos, ancho, chunk
    int n = args[0], d = args[1], c = args[2];
    struct nodo *L=0;

    while(endflag == 0){

        pthread_mutex_lock(&Leer);          //Entrando a sección crítica

        if(endflag == 0){                  //Si la endflag esta en 0 quedan datos por leer
            L = CS_readChunk(c);           //Sección de lectura del archivo (Solo una hebra puede leer a la vez)
            pthread_mutex_unlock(&Leer);    //Saliendo de la sección crítica
        }
        else{     //Si enflag esta en 1 entonces una hebra ya terminó de leer el archivo por lo que las demás deben terminar su ejecución                    
            pthread_mutex_unlock(&Leer);     //Debemos salir de la sección crítica antes de terminar la ejecución de las hebras por si hay otras esperando
            return; 
        }

       
        //Si llegamos a este punto es porque hay datos por procesar
        procesar_chunk(L, n , d);


        L = 0;  //Reiniciamos L
    }
}


