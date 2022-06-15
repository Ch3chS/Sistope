#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "math.h"
double Real[], Imaginaria[], Ruido[];  //Estos arreglos guardarán los datos necesarios para los calculos
// ------------------------------------------------------ Función de lectura ----------------------------------------------------------------------------------
//Entrada: Entero con la dirección de lectura correspondiente a este hijo
//Salida: Un entero con el número de datos recibidos
//Función encargada de leer todos los datos de la pipe
int lectura(int Lectura){
    double temp[5] = {0,0,0,0,0};
    int i = 0;
    while(temp[4] != -1){         //Este ciclo se ejecutará hasta recibir el mensaje de FIN (-1 en el ruido)
        read(Lectura,temp,40);    //Se lee la pipe
        if(temp[4] == -1){
            break;
        }
        Real[i] = temp[2];        //Se guardan los datos en los arreglos correspondientes
        Imaginaria[i] = temp[3];  //(algo a tener en cuenta es que se leerá hasta que se hayan mandado TODAS las visibilidades correspondientes)
        Ruido[i] = temp[4];
        i++;
    }
    return i+1;  //Retornamos el número total de datos (i parte desde el 0 asi que el número total seria i+1)
}
// ------------------------------------------------------ Funciones de procesamiento ---------------------------------------------------------------------------
//Entrada: Un arreglo con valores y un entero el largo del arreglo
//Salida: Un número de tipo double que contiene la media de los valores del arreglo
//Calcula la media de un arreglo de n valores
double Media(double valores[], int n){
    double media = 0;  //Aquí se guardarán los valores que va tomando la media a lo largo del calculo
    for(int i = 0; i < n; i++){   //Recorremos el arreglo
        media += valores[i];      //Sumamos todos los valores del mismo
    }
    media /= n;   //Una vez sumados todos dividimos el resultado por el número de valores
    return media; //Retornamos el resultado
}
//Entrada: 2 arreglos con valores tipo double y el largo de ambos (poseen el mismo)
//Salida: La potencia como tipo double
//Funcion que calcula la potencia a partir de 2 arreglos con valores (potencia = sum((x^2 + y^2)^(1/2)))
double Potencia(double real[], double imaginaria[], int n){
    double p = 0;  //Aquí se guardarán los valores que va tomando la potencia a lo largo del calculo
    for(int i = 0; i < n; i++){  //Recorremos ambos arreglos posición a posición
        p += sqrt(pow(real[i], 2) + pow(imaginaria[i], 2));  //Aquí hacemos la raiz cuadrada de la suma de los cuadrados y sumamos el resultado de eso a lo que se llevaba
    }
    return p;  //Ya habiendo sumado el calculo hecho en todas las posiciones se retorna el resultado
}
//Entrada: Un arreglo con los ruidos asociados a los datos y un entero con el largo del arreglo
//Salida: El ruido total como tipo double
//Suma todos los ruidos incluidos en el arreglo
double Ruido_total(double ruido[], int n){
    double total = 0;   //Aquí se guardarán las sumas
    for(int i = 0; i < n; i++){   //Recorremos el arreglo
        total += ruido[i];        //Sumamos la nueva posición a las anteriores
    }
    return total;  //Una vez sumadas todas se retorna el resultado
}
//Entrada: Direcciones de las pipes de lectura y escritura
//Salida: No llega a devolver el entero porque termina antes el proceso, pero si no lo hiciera devolveria uno diciendo que la función termino correctamente
//Función principal del proceso hijo post-execve; realmente solo llama a las otras funciones
int main(int argc, char *argv[]){

    int Lectura = atoi(argv[1]);      //Tanto el valor de la dirección de lectura como de escritura deben ser pasadas a entero nuevamente
    int Escritura = atoi(argv[2]);

    //Ciclo de lectura y procesamiento
    int n = lectura(Lectura);   //Se leen y almacenan los datos en los distintos arreglos
    
    //Procesamiento
    double resultados[4]= {0,0,0,0};               //En este arreglo se guardarán los datos de las 4 operaciones a realizar
    resultados[0] = Media(Real ,n);                //Las 2 primeras se hacen con la misma función, pues ambas son medias aritméticas
    resultados[1] = Media(Imaginaria ,n); 
    resultados[2] = Potencia(Real, Imaginaria, n); //Se calcula la potencia
    resultados[3] = Ruido_total(Ruido, n);         //Se suman los ruidos
    
    //Envio de resultados
    write(Escritura,resultados,40);               //Una vez se tienen todos los resultados se envian al proceso padre

    //Finalizar proceso hijo
    close(0);                                     //Ya habiendo realizado su valor el proceso hijo termina
    return 0;
}