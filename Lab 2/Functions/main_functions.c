#include "../headers.h"


//Entrada: Nombre de un archivo como cadena de caracteres
//Salida: No posee
//Abre el archivo sin escribir nada en el, y avisa en caso de un error al abrir el mismo
void file_open(char *file_name){
    fp = fopen(file_name,"r");      //Se intenta abrir el archivo 

    if(fp == NULL){                //Si no se encuentra nada entonces se avisa del error y se detiene el programa (lo más probable es que el nombre este mal ingresado)
        perror("Error en la apertura del archivo");
        exit(1);
    }
}


//Entrada: Número de anillos
//Salida: No posee
//Inicializa los discos con sus valores en 0
void inicializar_anillos(int n){
    for(int i = 0; i < n; i++){
        discos[i].media_r = 0;
        discos[i].media_i = 0;
        discos[i].potencia = 0;
        discos[i].ruido = 0;
        discos[i].visibilidades_totales = 0;
    }
}


//Entrada: Entero con el número de discos
//Salida: No posee
//Inicializa los diferentes semaforos a usar a lo largo del programa para asi evitar problemas de concurrencia
void semaphores_init(int n){
    pthread_mutex_init(&Leer, NULL);                 //Inicializamos el semaforo de lectura del archivo
    pthread_mutex_init(&Arreglo_edit_disk, NULL);  
    for(int i = 0; i < n; i++){                      //Para cada disco
        pthread_mutex_init(&Edit_disk[i], NULL);     //Inicializamos su respectivo semaforo propio
    }
}


//Entrada: Un entero con el número de hebras y un arreglo de enteros con los argumentos a usar por estas
//Salida: No posee
//Crea las hebras pasandoles el argumento solicitado (La id de las hebras queda guardada en un arreglo para la siguiente función)
void threads_creation(int h, int args[3]){
    
    for(int i = 0; i < h;i++){     //Con el número de hebras ingresado por consola realizamos un ciclo
        pthread_create(&threads[i], NULL, thread, args);   //Por cada iteración creamos una hebra
    }
}


//Entrada: El número de hebras (También hace uso del arreglo rellenado en la función previa)
//Salida: No posee
//Espera a que finalicen todas las hebras una por una
void wait_threads(int h){
    for(int i = 0; i < h; i++){
        void *retval;
        pthread_join(threads[i], &retval);   //Esperamos a que terminen las hebras una por una en orden
    }   
    //Si salimos del for es porque ya todas terminaron por lo que dejamos de esperar.
}


//Entrada: Entero con el número de discos
//Salida: No posee
//Divide lo acumulado en la media real e imaginaria de cada disco (A menos que estos no tengan ninguna visibilidad dentro)
void average_division(int n){
    for(int i = 0; i < n;i++){                                                            //Para cada disco
        if(discos[i].visibilidades_totales != 0){                                         //Esto es por si no hay ninguna visibilidad en un disco
            discos[i].media_r = (discos[i].media_r / discos[i].visibilidades_totales);    //suma de visibilidades reales / número de visibilidades 
            discos[i].media_i = (discos[i].media_i / discos[i].visibilidades_totales);    //suma de visibilidades imaginarias / número de visibilidades 
        }
    }
}


//Entrada: El nombre de un archivo a modo de cadena de caracteres, un entero con el número de discos y una flag que nos dice si debemos imprimir o no por pantalla
//Salida: No posee
//Escribe los datos obtenidos en un archivo con un nombre dado, y, en caso de pedirse por consola con "-b" también se debe imprimir lo obtenido en la misma
void write_file(char *file_name, int n, int b){
    FILE *fs;
    fs = fopen(file_name,"w");
    for(int i = 0; i < n; i++){  //Para cada disco
        fprintf(fs,"Disco %d\n", i+1);                    //Escribimos los datos solicitados
        fprintf(fs,"Media real: %f\n", discos[i].media_r);
        fprintf(fs,"Media imaginaria: %f\n", discos[i].media_i);
        fprintf(fs,"Potencia: %f\n", discos[i].potencia);
        fprintf(fs,"Ruido total: %f\n", discos[i].ruido);
        if(b == 1){                    //Si se escribió la bandera -b al llamar al programa el contenido también se verá en la terminal
            printf("Disco %d:\n", i+1);
            printf("Media real: %f\n", discos[i].media_r);
            printf("Media imaginaria: %f\n", discos[i].media_i);
            printf("Potencia: %f\n", discos[i].potencia);
            printf("Ruido total: %f\n", discos[i].ruido);
        }
    }
}
