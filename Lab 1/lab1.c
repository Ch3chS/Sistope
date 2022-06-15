#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "math.h"

int comunicaciones[], pids[];
double media_r[], media_i[], potencia[], ruido[];


// ------------------------------------------------------ Funciones de la FASE 1 ---------------------------------------------------------------------------

//Entrada: Entero con el número de discos
//Salidas: Entero avisando si la función termino bien
//Crea a los procesos hijos y cierra las pipes que el proceso correspondiente no usará; luego los hijos cambian de imagen haciendo uso de execve
int hijos(int n){
    int pid;
    for(int i = 0; i < n; i++){                    //En este for se crearán los hijos
        if ( (pid = fork()) == -1) {
            printf("No se pudo crear hijo\n");     
            exit(-1);                              
        }
        
        pids[i] = pid;    //Guardamos el pid del hijo de ser necesario más adelante                       
        if (pid == 0) { // soy el hijo                  //Las pipes para los hijos funcionarán de la siguiente forma:
            for(int j = 0; j < n; j++){                //El modulo de lectura con posición i en comunicaciones[] será usada para que los hijos lean
                if(j != i && j != i+(3*n)){           //Y el de escritura en posición i+3*n será usada para que estos escriban
                    close(comunicaciones[j]);        //(recordar que i y i+2*n son la misma pipe al igual que i+n y i+3*n (siendo i e i+n quienes leen y los otros escriben))
                }                                   //Por lo tanto cerramos los canales que no cumplan con esto.
            }
            
            char text[20], text2[20];                  //Convertimos los enteros de las pipes a char para pasarlos como argumentos
            sprintf(text, "%d", comunicaciones[i]);
            sprintf(text2, "%d", comunicaciones[i+(3*n)]);

            char *argv2[] = {"Vis",text, text2,NULL};  //Preparamos los argumentos para el cambio de imagen
            char *newenviron[] = {NULL};
            
            execve("Vis", argv2, newenviron);           //Ejecutamos el cambio
            perror("execve");
            exit(EXIT_FAILURE);
        }
        else { // soy el padre
            close(comunicaciones[i]);      //Cerramos los canales a usar por el hijo recien creado
            close(comunicaciones[i+(3*n)]);
        }
    }
    return 0;
}




// ------------------------------------------------------ Funciones de la FASE 2 ---------------------------------------------------------------------------

//Entrada: String con el nombre del archivo   (los otros 2 son solo para pasarlos a la siguiente función)
//Salida: Un entero que indique como termino la función
//Esta función lee linea por linea del archivo .csv y por cada una llama a la siguiente función
int leer_archivo(char *nombre, int n, int d){
    FILE *fp;
    char *cadena = NULL;   //Tanto la cadena a guardar como el número de bytes inician en nulo y 0 para que la misma getline asigne el espacio
    size_t bytes = 0;
    fp = fopen(nombre,"r");                        //Abrimos el archivo en modo lectura
    
    while(getline(&cadena, &bytes, fp) != -1){     //Leemos cada una de las lineas del archivo
        procesar_linea(cadena, n, d);              //Para cada linea llamamos a la función encargada de procesarla
    }
    
    return 0;    //Una vez se leyeron y procesaron todas las lineas termina la función
}

//Entrada: una cadena de caracteres correspondiente a una linea del archivo   (los otros 2 son solo para pasarlos a la siguiente función)
//Salida: un entero que indica como termino la función
//Esta función se encargará de procesar la linea entrante separando sus valores y convirtiendolos en tipo float para pasarselo a la siguiente función
int procesar_linea(char *linea, int n, int d){
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
    
    seleccionar_y_enviar(valores, n , d);
    return 0;
}

//Entrada: Datos de la linea del archivo x numero de discos x radio del disco
//Salida: Un entero que indica como termino la función
//Selecciona el disco encargado a pae
int seleccionar_y_enviar(double datos[5], int n_discos, int radio){
    int distancia = sqrt(pow(datos[0], 2) + pow(datos[1], 2)); // Modulo (raiz de la suma de los cuadrados de u y v)
    int i = 1;  //La i será un iterador para ubicarnos entre los radios de los distintos discos y asi saber en que disco estamos ubicados
    while(i < n_discos){ 
        if((((i-1)*radio) <= distancia) && (distancia < (i*radio))){  //Se verifica la pertenencia a un disco
            //Numero de disco dado por i
            
            write(comunicaciones[(i-1) + (2 * n_discos)], datos, 40);
            return 0; //Si ya se encontro el disco y se mandaron los datos retornamos para continuar
        }
        i++;   // Si no entra en el rango de este disco pasamos al siguiente
    }
    //Si salimos del bucle es porque la distancia supera el limite del penultimo disco, es decir pertenece a la capa exterior
    //Entonces el numero de disco dado por n_disco (El ultimo disco)
    write(comunicaciones[3* n_discos - 1], datos, 40);  // Recordar que la escritura del padre va desde el 2*n hasta el 3*n-1 siendo el ultimo el disco más grande
    return 0;
}




// ------------------------------------------------------ Funciones de la FASE 3 ---------------------------------------------------------------------------

//Entrada: Número de discos
//Salida: Entero
//Función que envia un mensaje de fin a todos los procesos hijos
int fin(int n){
    double fin[5] = {0,0,0,0,-1};   //El mensaje de fin será un arreglo con ceros y un -1 en la ultima posición
    for(int i = 0; i < n; i++){
        write(comunicaciones[i+(2*n)],fin,40);
    }
    return 0;
}




// ------------------------------------------------------ Funciones de la FASE 4 ---------------------------------------------------------------------------

//Entrada: Entero con el número de discos
//Salida: Entero
//Función que recibe y almacena los resultados enviados por los procesos hijos
int resultados(int n){
    double temp[4] = {0,0,0,0};  //Aquí se guardarán temporalmente los recibidos en cada mensaje
    
    for(int i = 0; i < n; i++){
        wait(pids[i]);   //Esperamos a que el proceso hijo finalice
        read(comunicaciones[i + n],temp,40);  //Leemos el pipe
        media_r[i] = temp[0];  //Guardamos en la posición del número de disco - 1 los resultados correspondientes en cada arreglo
        media_i[i] = temp[1];
        potencia[i] = temp[2];
        ruido[i] = temp[3];
    }
   
    return 0;  //Retornamos un 0 para decir que termino bien la función
}

//Entrada: Nombre del archivo, número de discos, Flag de si se debe ver el contenido 
//Salida: Entero
//Lee los resultados entregados por los hijos e imprime un documento a partir de estos (Podría también imprimirlos por pantalla)
int imprimir_archivo(char *nombre, int n, int b){
    FILE *fp;
    fp = fopen(nombre,"w");
    for(int i = 0; i < n; i++){  //Para cada disco
        fprintf(fp,"Disco %d\n", i+1);                    //Escribimos los datos solicitados
        fprintf(fp,"Media real: %f\n", media_r[i]);
        fprintf(fp,"Media imaginaria: %f\n", media_i[i]);
        fprintf(fp,"Potencia: %f\n", potencia[i]);
        fprintf(fp,"Ruido total: %f\n", ruido[i]);
        if(b == 1){                    //Si se escribió la bandera -b al llamar al programa el contenido también se verá en la terminal
            printf("Disco %d:\n", i+1);
            printf("Media real: %f\n", media_r[i]);
            printf("Media imaginaria: %f\n", media_i[i]);
            printf("Potencia: %f\n", potencia[i]);
            printf("Ruido total: %f\n", ruido[i]);
        }
    }
    return 0;
}




// ----------------------------------------------------- MAIN (FASE 0) -----------------------------------------------------------------------------------------

//Entradas: Argumentos de consola
//Salidas: Entero
//Descripción: Función principal, en un principio se encarga de recibir los argumentos, comprobarlos y almacenarlos para luego llamar funciones.
int main(int argc, char *argv[]){
    
    //FASE 0: Inicialización
    //A CONTINUACIÓN SE VERIFICAN Y SE GUARDAN LOS VALORES INGRESADOS POR CONSOLA
    extern char *optarg;
    extern int optind;
    int c, err = 0;    //c es temporal para ver los parametros de entrada en el ciclo while de abajo y err es una flag por si se ingresa algo que no se debería
    int iflag = 0, oflag = 0, dflag = 0, nflag = 0, bflag = 0;   //Estas flags servirán para saber si un parametro se ingreso o no
    char *iname = "VALORES_I", *oname = "VALORES_O", *dname = "VALORES_D", *nname = "VALORES_N";   //Aquí se guardaran los argumentos de los distintos parametros
    char usage[] = "usage: %s -i \"0 VALORES\" -o \"0 VALORES\" -d \"0 VALORES\" -n \"0 VALORES\"\n"; //Esto es solo para guardar el formato que se deberia ingresar
    while ((c = getopt(argc, argv, "i:o:d:n:b")) != -1)   //Aca es donde se analizan los argumentos uno por uno
    {
        switch(c)
        {
        case 'i':
            iflag = 1;          //Si se ve un un parametro como -i seteamos el 1 en la flag para decir que efectivamente se ingreso
            iname = optarg;     //Luego en caso de ser necesario guardamos el argumento post flag
            break;
        case 'o':
            oflag = 1;
            oname = optarg;
            break;
        case 'd':
            dflag = 1;
            dname = optarg;
            break;
        case 'n':
            nflag = 1;
            nname = optarg;
            break;
        case 'b':
            bflag = 1;        //En el caso particular de b no nos interesa el argumento sino solo saber si se puso o no
            break;
        case'?':
            err = 1;
            break;
        }
    }
    //Si falta una bandera se avisa que falta ese argumento
    if(iflag == 0){
        fprintf(stderr,"%s: missing -i option\n",argv[0]);
        fprintf(stderr,usage, argv[0]);
        exit(1);    
    }
    if(oflag == 0){
        fprintf(stderr,"%s: missing -o option\n",argv[0]);
        fprintf(stderr, usage, argv[0]);
        exit(1);    
    }
    if(dflag == 0){
        fprintf(stderr,"%s: missing -d option\n",argv[0]);
        fprintf(stderr,usage, argv[0]);
        exit(1);    
    }
    if(nflag == 0){
        fprintf(stderr,"%s: missing -n option\n",argv[0]);
        fprintf(stderr, usage, argv[0]);
        exit(1);    
    }
    if(optind<argc){                          //Si hay argumentos de sobra o algo también se avisa y se repite la sintaxis para que el usuario vuelva a intentarlo
        for (; optind<argc; optind++){
            printf("Argumento no permitido:\"%s\"\n", argv[optind]);
            printf("La sintaxis es: -i \"VALOR X\" -o \"X VALOR\" -d \"0 VALORES\" -n \"0 VALORES\"\n");
            exit(1);
        }
        
    }
    //Si la bandera de error esta activada avisamos y decimos la sintaxis
    if(err){
        fprintf(stderr, usage, argv[0]);
        exit(1);
    }
    // Convertimos d y n a enteros para usarlos
    int d = atoi(dname);
    int n = atoi(nname);
    //YA HABIENDO OBTENIDO LAS ENTRADAS SE PROCEDERA CON LA OPERACIÓN
    //FASE 1: Creación de pipes e hijos
    int p[2];
    for(int i = 0; i < n*2; i++){       //En este for se crearán las pipes necesarias (2 por disco)
        if(pipe(p) == -1){
            printf("No se pudo crear pipe\n");
        }
        else{
            comunicaciones[i] = p[0];           //De esta manera en el arreglo desde el 0 a 2*n-1 quedan todos los canales de lectura
            comunicaciones[i+(2*n)] = p[1];    //Y desde el 2*n al 4*n-1 quedan todos los de escritura
                                            //La pipe a usar depende del i siendo i para leer y i+2*n para escribir en la misma
        }
    }
    hijos(n);  //En esta función se crearán los hijos.
    //FASE 2: Lectura y enviado de datos
    leer_archivo(iname, n, d);

    //FASE 3: Mensaje de FIN
    fin(n);
    
    //FASE 4: Leer resultados e imprimir archivo de salida
    resultados(n);
    
    imprimir_archivo(oname, n, bflag);
    //FASE 5: Cerrar y finalizar
    return 0;
}
