#include "cola.h"
#include <stdlib.h>

//Definición del struct nodo
struct nodo;
typedef struct nodo nodo_t;

struct nodo {
    void* dato; // apunta al dato almacenado
    nodo_t* prox;   //apunta al próximo nodo
};

//Definición del struct cola
struct cola {
    void **datos;
    nodo_t* primero;   // apunta al primer nodo de la cola
    nodo_t* ultimo;   // apunta al último nodo de la cola
};

nodo_t *crear_nodo(void) {
    nodo_t *nodo = malloc(sizeof(nodo_t));
    if (!nodo) return NULL;
    nodo->dato = NULL;
    nodo->prox = NULL; // Como al crear un nodo también creo un Nodo Próximo tengo que inicializarlo con algun valor
    return nodo;
}
//void nodo_destruir(nodo_t *nodo) {
//    free(nodo);
//} 

/* *****************************************************************
 *                    PRIMITIVAS DE LA COLA
 * *****************************************************************/

cola_t *cola_crear(void) {
    cola_t *cola = malloc(2*sizeof(cola_t));
    if (cola == NULL) {
        return NULL;
    }
    cola->primero = NULL;
    cola->ultimo = NULL;
    return cola;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)) {
    while (!cola_esta_vacia(cola)) {
        if (destruir_dato != NULL) {
            destruir_dato(cola_desencolar(cola)); //Al desencolar destruyo el nodo y devuelvo la estructura (ej: pila, persona)
        }
        else {
            cola_desencolar(cola);
        }
    }
    free(cola);
}

void *cola_ver_primero(const cola_t *cola) {
    if (cola_esta_vacia(cola)) {
        return NULL;
    }
    return cola->primero->dato;
}

bool cola_esta_vacia(const cola_t *cola) {
    return cola->primero == NULL;
}

bool cola_encolar(cola_t *cola, void *valor) {
    nodo_t *nodo = crear_nodo();
    if (!nodo) return false;
    nodo->dato = valor;
    if (cola->ultimo == NULL) {
        cola->primero = nodo;
        cola->ultimo = nodo;
    }
    else {
        cola->ultimo->prox = nodo;
        cola->ultimo = cola->ultimo->prox;
    }
    return true;
}

void *cola_desencolar(cola_t *cola) {
     if (cola_esta_vacia(cola)) {
        return NULL;
    }
    void* nodo_a_desencolar = cola->primero;
    void* dato_a_desencolar = cola->primero->dato;
    cola->primero = cola->primero->prox;
    if (cola->primero == NULL) {
        cola->ultimo = NULL;
    }
    free(nodo_a_desencolar);
    return dato_a_desencolar;
}