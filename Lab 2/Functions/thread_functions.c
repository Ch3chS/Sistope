#include "../headers.h"


//------------------------------------------  Secciones críticas  ------------------------------------------------------

struct leido CS_readChunk(int chunk){
    char *cadena = NULL;   //Tanto la cadena a guardar como el número de bytes inician en nulo y 0 para que la misma getline asigne el espacio
    size_t bytes = 0;

    struct leido r;
    r.largo_lista = 0;
    
    printf("%d\n",chunk);

    for(int i = 0; i < chunk; i++){
        if(getline(&cadena, &bytes, fp) == -1){   //Si llegamos al final del documento
            endflag = 1;                          //Le avisamos a las demas hebras que el archivo ha terminado
            break;                                //Salimos del bucle para no continuar intentando leer
        }
        r.lineas[i] = cadena;                       //Si no se ha terminado guardamos la linea para procesarla despues
        r.largo_lista += 1;
        //printf("%d\n",i);
    }

    return r;    //Una vez se leyeron y procesaron todas las lineas termina la función
}

void CS_actualizarAnillo(){

}




//----------------------------------------------  Funciones  -----------------------------------------------------------


void actualizarPropiedades(){

}




void thread(int args[3]){ //int N_discos, ancho, chunk
    int n = args[0], d = args[1], c = args[2];
    struct leido r;

    while(endflag == 0){
        //Semaforo binario
        r = CS_readChunk(c);
        //Liberar semaforo

        if(r.largo_lista != 0){
            //printf("%d\n", r.largo_lista);
            actualizarPropiedades();
        }
    }


}


