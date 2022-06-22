#include "../headers.h"


//Estoy reutilizando y modificando parte de un TDA que nos enseñaron a crear en el ramo estructura de datos.


//Entrada: una cadena de caracteres
//Salida: Un nodo para una lista simplemente enlazada
//Crea un nodo con su respectivo dato y lo retorna para su uso
struct nodo *crear_nodo(char *dato){
    // Crea un nodo y devuelve la referencia al mismo
    struct nodo *aux;                             // declaramos donde guardaremos la referencia del nodo
    aux = (struct nodo *)malloc(sizeof(struct nodo));    // asignamos memoria y guardamos el puntero al nodo
    aux->dato = dato;                      // guardamos el dato en el nodo
    aux->sig = NULL;                       // conectamos el extremo del nodo al null (ultimo en la lista)
    return aux;                            // retornamos la referencia al nodo creado
}


//Entrada: Una cadena de caracteres y una lista enlazada
//Salida: Una lista enlazada
//Crea un nodo con el dato haciendo uso de la función anterior y lo agrega al inicio de la lista
struct nodo *agregar_nodo(char *nuevo_dato, struct nodo *lista){  //Como en este caso el orden de las lineas no influye decidí usar la función agregar nodo al inicio que ocupa menos tiempo
    struct nodo *aux;
    aux = crear_nodo(nuevo_dato);
    aux->sig = lista;
    lista = aux;
    return lista;
}


//Entrada: Una lista enlazada
//Salida: Una lista enlazada
//Libera el primer nodo de la lista sin afectar al resto de esta
struct nodo *borrar_inicio(struct nodo *lista){      //Decidí agregar esta función para eliminar los nodos ya leídos
    if(lista != NULL){
        struct nodo *aux;
        aux = lista;
        lista = aux->sig;
        aux->sig = NULL;
        free(aux);
    }
    return lista;
}


//Entrada: Una lista enlazada
//Salida: Una cadena de caracteres
//Recibe una lista y retorna el dato del primer nodo
char *leer_nodo(struct nodo *lista){
    char *temp = lista->dato;
    return temp;
}