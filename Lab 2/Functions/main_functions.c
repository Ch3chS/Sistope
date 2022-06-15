#include "../headers.h"

void file_open(char *file_name){
    fp = fopen(file_name,"r");      //Se intenta abrir el archivo 

    if(fp == NULL){                //Si no se encuentra nada entonces se avisa del error y se detiene el programa (lo más probable es que el nombre este mal ingresado)
        perror("Error en la apertura del archivo");
        exit(1);
    }
}

void threads_creation(int h, int args[3]){
    
    for(int i = 0; i < h;i++){     //Con el número de hebras ingresado por consola realizamos un ciclo
        pthread_create(&threads[i], NULL, thread, args);   //Por cada iteración creamos una hebra
    }
}

void wait_threads(int h){
    for(int i = 0; i < h; i++){
        void *retval;
        pthread_join(threads[i], &retval);   //Esperamos a que terminen las hebras una por una en orden
    }   
    //Si salimos del for es porque ya todas terminaron por lo que dejamos de esperar.
}

void average_division(int n){
    for(int i = 0; i < n;i++){                                                        //Para cada disco
        discos[i].media_r = (discos[i].media_r / discos[i].visibilidades_totales);    //suma de visibilidades reales / número de visibilidades 
        discos[i].media_i = (discos[i].media_i / discos[i].visibilidades_totales);    //suma de visibilidades imaginarias / número de visibilidades 
    }
}

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
