#include "pila.h"
#include <stdlib.h>

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

// Defino capicidad inicial que tendrá la pila
#define CAPACIDAD_INICIAL 10
#define VALOR_REDIMENSION 2.0

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

// ...
pila_t *pila_crear(void) {
    pila_t *pila = malloc(sizeof(pila_t));
    if (pila == NULL) {
        return NULL;
    }
    pila->capacidad = CAPACIDAD_INICIAL;
    pila->cantidad = 0;
    pila->datos = malloc(CAPACIDAD_INICIAL*sizeof(void*));
    if (pila->datos == NULL) {
        free(pila);
        return NULL;
    }
    return pila;
}

void pila_destruir(pila_t *pila) {
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila) {
    return pila->cantidad == 0; //si pila->cantidad vale cero devuelve TRUE sino FALSE
}

bool pila_redimensionar(pila_t *pila, float multiplicador); //Le paso multiplicador = 2 para agrandar la pila y != 2 para achicarla

bool pila_apilar(pila_t *pila, void *valor) {
    if (pila->capacidad == pila->cantidad) {
        if (!pila_redimensionar(pila, VALOR_REDIMENSION)) { //le paso como parametro un 2.0 porque quiero duplicar la capacidad de la pila
            return false; // devuelve FALSE para avisar que falla la redimension de la pila
        }
    }
    pila->datos[pila->cantidad] = valor;
    pila->cantidad++;
    return true;
}

void *pila_ver_tope(const pila_t *pila) {
    if (pila_esta_vacia(pila)) {
        return NULL;
    }
    return pila->datos[pila->cantidad - 1]; //pongo el -1 porque cuento desde el cero
}

void *pila_desapilar(pila_t *pila) {
    if (pila_esta_vacia(pila)) {
        return NULL;
    }
    if (pila->capacidad >= 4*pila->cantidad && pila->capacidad > CAPACIDAD_INICIAL) {
        pila_redimensionar(pila, 0.5); //Le paso un parametro 0.5 porque quiero disminuir a la mita la capacidad de la pila
    }
    pila->cantidad--;
    return pila->datos[pila->cantidad]; //aca no pongo el -1 porque ya lo resté
}

bool pila_redimensionar(pila_t *pila, float multiplicador) {
        
    void *datos_nuevo = realloc(pila->datos, (unsigned int)((float)pila->capacidad * multiplicador*sizeof(void*)));
    if (datos_nuevo == NULL) { 
        //falló el realloc
        return NULL;
    }
    pila->capacidad = (unsigned int)((float)pila->capacidad * multiplicador); //La nueva capacidad es el doble o la mitad que la anterior

    pila->datos = datos_nuevo;
    return true;
}

