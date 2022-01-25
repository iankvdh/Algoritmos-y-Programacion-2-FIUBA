#include "pila.h"
#include <stdlib.h>

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

// ...
pila_t *pila_crear(void) {
    pila_t *pila = malloc(sizeof(pila_t));
    if (pila == NULL) {
        return NULL;
    }
    pila->capacidad = 10;
    pila->cantidad = 0;
    pila->datos = malloc(10*sizeof(void*));
    return pila;
}

void pila_destruir(pila_t *pila) {
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila) {
    if (pila->cantidad !=0) {
        return false;
    }
    return true;
}

bool pila_redimensionar(pila_t *pila, unsigned int multiplicador); //Le paso multiplicador = 2 para agrandar la pila y != 2 para achicarla

bool pila_apilar(pila_t *pila, void *valor) {
    if (pila->capacidad == pila->cantidad) {
        pila_redimensionar(pila, 2); //le paso como parametro un 2 para saber que quiero agrandar la pila
    }
    pila->datos[pila->cantidad] = valor;
    pila->cantidad++;
    return true;
}

void *pila_ver_tope(const pila_t *pila) {
    if (pila->cantidad == 0) {
        return NULL;
    }
    return pila->datos[pila->cantidad - 1]; //pongo el -1 porque cuento desde el cero
}

void *pila_desapilar(pila_t *pila) {
    if (pila->cantidad == 0) {
        return NULL;
    }
    if (pila->capacidad >= 4*pila->cantidad && pila->capacidad > 10) {
        pila_redimensionar(pila, 1); //Le paso un parametro != 2 para saber que quiero achicar la pila (OJO es de tipo int no sirve que pase 1/2)
    }
    pila->cantidad--;
    return pila->datos[pila->cantidad]; //aca no pongo el -1 porque ya lo resté
}

bool pila_redimensionar(pila_t *pila, unsigned int multiplicador) {
    //agrando la pila (pidiendo el doble del espacio que tenía hasta el momento)
    if (multiplicador == 2) {
        pila->capacidad = pila->capacidad * multiplicador; //La nueva capacidad es el doble que la anterior
        void *datos_nuevo = realloc(pila->datos, pila->capacidad*sizeof(void*));
        if (datos_nuevo == NULL) { 
            //falló el realloc
            return NULL;
        }
        pila->datos = datos_nuevo;
    }
    //achico la pila (pidiendo la mitad del espacio que tenía hasta el momento)
    else {
        pila->capacidad = pila->capacidad/2; //La nueva capacidad es la mitad que la anterior
        void *datos_nuevo = realloc(pila->datos, pila->capacidad*sizeof(void*));
        if (datos_nuevo == NULL) { 
            //falló el realloc
            return NULL;
        }
        pila->datos = datos_nuevo;
    }
    return true;
}

