#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "pthread.h"
#include "math.h"
#include "headers.h"


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
    int iflag = 0, oflag = 0, nflag = 0, dflag = 0, hflag = 0, cflag = 0, bflag = 0;   //Estas flags servirán para saber si un parametro se ingreso o no
    char *iname = "VALORES_I", *oname = "VALORES_O", *nname = "VALORES_N", *dname = "VALORES_D", *hname = "VALORES_H", *cname = "VALORES_C";   //Aquí se guardaran los argumentos de los distintos parametros
    char usage[] = "usage: %s -i \"0 VALORES\" -o \"0 VALORES\" -n \"0 VALORES\" -d \"0 VALORES\" -h \"0 VALORES\" -c \"0 VALORES\"\n"; //Esto es solo para guardar el formato que se deberia ingresar
    while ((c = getopt(argc, argv, "i:o:n:d:h:c:b")) != -1)   //Aca es donde se analizan los argumentos uno por uno
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
        case 'n':
            nflag = 1;
            nname = optarg;
            break;
        case 'd':
            dflag = 1;
            dname = optarg;
            break;
        case 'h':
            hflag = 1;
            hname = optarg;
            break;
        case 'c':
            cflag = 1;
            cname = optarg;
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
    if(nflag == 0){
        fprintf(stderr,"%s: missing -n option\n",argv[0]);
        fprintf(stderr, usage, argv[0]);
        exit(1);    
    }
    if(dflag == 0){
        fprintf(stderr,"%s: missing -d option\n",argv[0]);
        fprintf(stderr,usage, argv[0]);
        exit(1);    
    }
    if(hflag == 0){
        fprintf(stderr,"%s: missing -h option\n",argv[0]);
        fprintf(stderr, usage, argv[0]);
        exit(1);    
    }
    if(cflag == 0){
        fprintf(stderr,"%s: missing -c option\n",argv[0]);
        fprintf(stderr, usage, argv[0]);
        exit(1);    
    }
    if(optind<argc){                          //Si hay argumentos de sobra o algo también se avisa y se repite la sintaxis para que el usuario vuelva a intentarlo
        for (; optind<argc; optind++){
            printf("Argumento no permitido:\"%s\"\n", argv[optind]);
            printf("usage: %s -i \"0 VALORES\" -o \"0 VALORES\" -n \"0 VALORES\" -d \"0 VALORES\" -h \"0 VALORES\" -c \"0 VALORES\"\n");
            exit(1);
        }
        
    }
    //Si la bandera de error esta activada avisamos y decimos la sintaxis
    if(err){
        fprintf(stderr, usage, argv[0]);
        exit(1);
    }
    
    // Convertimos los datos enteros que necesitamos
    nname = atoi(nname);         //Si los enteros fueron ingresados con caracteres el atoi devolvera 0 por lo que sabremos si esto ocurre
    hname = atoi(hname);         //(tener en cuenta que por la naturaleza de los valores necesarios los cuatro deben ser mayores a 0)
    cname = atoi(cname);
    dname = atoi(dname);

    if(nname <= 0 || hname <= 0 || cname <= 0 || dname <= 0){    //Si alguno fue ingresado con caracteres o son menores o iguales a 0 se menciona el error y se detiene el programa
        printf("Valores ingresados de forma incorrecta\nPor favor volver a intentar\n");
        exit(1);
    }

    //--------------------------  Una vez verificados y guardados los valores ingresados por consola procedemos: ----------------------------------------------------

    file_open(iname);       
    
    int args[3] = {nname, dname, cname};
    threads_creation(hname, args);       //Se crean las hebras



    wait_threads(hname);          //Esperamos a que las hebras finalicen su trabajo

    //average_division(nname);

    //write_file(oname, nname, bflag);

    return 0;
}
