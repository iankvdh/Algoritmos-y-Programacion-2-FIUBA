#include <stdlib.h>
#include "abb.h"
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "pila.h"

/*
cd /home/ian/Documentos/Lista
gcc -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -o pruebas *.c
valgreen --leak-check=full --track-origins=yes --show-reachable=yes ./pruebas
*/

typedef struct nodo {
    struct nodo* izq;
    struct nodo* der;
    char* clave;
    void* dato;
} nodo_t;

struct abb {
    nodo_t* raiz;
    size_t cantidad;
    abb_comparar_clave_t cmp;
    abb_destruir_dato_t destruir_dato;
};

typedef bool (*_abb_guardar_t) (nodo_t **, const char *, void *, abb_t *);

/* 
* Recibe un doble puntero a un Nodo, una clave y un ABB y devuelve un doble puntero al Nodo con la clave dada.
 * En caso de que la clave no esté en el ABB devuelve NULL.
 * Si además recibe una funcion de guardar y un dato, inserta  el conjunto clave dato en el ABB (cumpliendo la propiedad de ABB)
 * En caso de que falle la insersión devuelve NULL.
 */ 
nodo_t** _buscar_ref(nodo_t **nodo, const char *clave, abb_t* arbol, _abb_guardar_t guardar, void* dato);


abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
    abb_t* abb = calloc(1, sizeof(abb_t));
    if (!abb) {
        return NULL;
    }

    abb->cmp = cmp;
    abb->destruir_dato = destruir_dato;

    return abb;
}

bool _agregar_hijo(nodo_t **padre, const char *clave, void *dato, abb_t *arbol) {
    nodo_t* nodo = calloc(1, sizeof(nodo_t));
    if (!nodo) {
        return false;
    }
    // Copiar clave
    nodo->clave = malloc(strlen(clave) + 1);
    if (!nodo->clave) {
        return false;
    }
    strcpy(nodo->clave, clave);
    // Copiar dato
    nodo->dato = dato;
    // Unir al arbol
    *padre = nodo;
    arbol->cantidad++;
    return true;
}

bool _abb_guardar(const char *clave, void *dato, abb_t *arbol) {
    // Caso clave que ya inserrtada
    if (abb_pertenece(arbol, clave)) {
        nodo_t** nodo = _buscar_ref(&arbol->raiz, clave, arbol, NULL, NULL);
        if (arbol->destruir_dato) {
            arbol->destruir_dato((*nodo)->dato);
        }
        (*nodo)->dato = dato;
    }
    //Caso clave nueva
    else {
        //Inserta el conjunto clave dato en el ABB
        if (!_buscar_ref(&arbol->raiz, clave, arbol, _agregar_hijo, dato)) {
            return false;
        }
    }  
    return true;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
    return _abb_guardar(clave, dato, arbol);
}

nodo_t** _buscar_ref(nodo_t **nodo, const char *clave, abb_t* arbol, _abb_guardar_t guardar, void* dato) {
    if (!*nodo) {
        if (!guardar || !guardar(nodo, clave, dato, arbol)){
            //En caso de no haber funcion de guardar o no haber podido guardar devuelvo NULL
            return NULL;
        }
        return nodo;
    }
    if (arbol->cmp(clave, (*nodo)->clave) > 0) {
        return _buscar_ref(&(*nodo)->der, clave, arbol, guardar, dato);
    } else if (arbol->cmp(clave, (*nodo)->clave) < 0) {
        return _buscar_ref(&(*nodo)->izq, clave, arbol, guardar, dato);
    }
    return nodo;
}

bool _es_hoja(nodo_t * nodo) {
    return !nodo->izq && !nodo->der;
}

nodo_t* _buscar_reemplazante(nodo_t * nodo) {
    //Siempre que llamamos a buscar reemplazante, sabemos que ese nodo no es hoja y tiene dos hijos
    nodo_t* nodo_reemplazante =nodo->izq;
    while (nodo_reemplazante->der) {
        nodo_reemplazante = nodo_reemplazante->der;
    }
    return nodo_reemplazante;
    
    nodo_reemplazante = nodo->der;
    while (nodo_reemplazante->izq) {
        nodo_reemplazante = nodo_reemplazante->izq;
    }
    return nodo_reemplazante;
}

void *abb_borrar(abb_t *arbol, const char *clave) {
    if (abb_cantidad(arbol) == 0) {
        return NULL;
    }
    nodo_t** ref = _buscar_ref(&arbol->raiz, clave, arbol, NULL, NULL);

    if (!ref) {
        return NULL;
    }
    nodo_t* nodo_a_borrar = *ref;


    void* dato = nodo_a_borrar->dato;

    //TIENE 2 HIJOS
    if (nodo_a_borrar->izq && nodo_a_borrar->der) {
        // Busco, guardo info y borro el reemplazante
        nodo_t* nodo_reemplazante = _buscar_reemplazante(nodo_a_borrar);
    
        char* clave_remplazate = malloc(strlen (nodo_reemplazante->clave) + 1);
        strcpy(clave_remplazate, nodo_reemplazante->clave);
        
        void* dato_reemplazante = abb_borrar(arbol, nodo_reemplazante->clave);

        // Borro la clave del nodo a borrar
        free(nodo_a_borrar->clave);

        // Paso los datos del reemplazante al nuevo espacio vacio
        nodo_a_borrar->clave = clave_remplazate;
        nodo_a_borrar->dato = dato_reemplazante;

        return dato;
    }
    else {
        if (nodo_a_borrar->izq) {
            *ref = nodo_a_borrar->izq;
        }
        else { // Si no hay der queda apuntando a NULL (nodo null)
            *ref = nodo_a_borrar->der;
        }
    }
    free(nodo_a_borrar->clave);
    free(nodo_a_borrar);
    arbol->cantidad--;
    return dato;
}

void *abb_obtener(const abb_t *arbol, const char *clave) {
    nodo_t** ref = _buscar_ref((nodo_t **)&arbol->raiz, clave, (abb_t*)arbol, NULL, NULL);
    return ref ? (*ref)->dato : NULL;
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
    return  _buscar_ref((nodo_t **)&arbol->raiz, clave, (abb_t*)arbol, NULL, NULL);
}

size_t abb_cantidad(const abb_t *arbol) {
    return arbol->cantidad;
}

void _abb_destruir_recursivo(nodo_t *nodo, abb_t *arbol) {
    if (!nodo) {
        return;
    }
    _abb_destruir_recursivo(nodo->izq, arbol);
    _abb_destruir_recursivo(nodo->der, arbol);

    if (arbol->destruir_dato) {
        arbol->destruir_dato(nodo->dato);
    } 
    free(nodo->clave);
    free(nodo); 
}

void abb_destruir(abb_t *arbol) {
    _abb_destruir_recursivo(arbol->raiz, arbol);
    free(arbol);
}

/////////////////////
// Iterador interno//
/////////////////////

bool _abb_in_order(nodo_t *nodo, bool visitar(const char *, void *, void *), void *extra) {
    if (!nodo) return true;

    bool izq = _abb_in_order(nodo->izq, visitar, extra);
    if (!izq) {
        return false;
    }
    if (!visitar(nodo->clave, nodo->dato, extra)) {
        return false;
    }
    return _abb_in_order(nodo->der, visitar, extra);

    return false;
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
    _abb_in_order(arbol->raiz, visitar, extra);
}

/////////////////////
// Iterador externo//
/////////////////////

struct abb_iter  {
    const abb_t* arbol;
    pila_t* pila;
};

bool _iter_apilar(pila_t* pila, nodo_t* nodo) {
    while (nodo) {
        if (!pila_apilar(pila, nodo)) return false;
        nodo = nodo->izq;
    }
    return true;
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
    abb_iter_t* iter = malloc(sizeof(abb_iter_t));
    if (!iter) {
        return NULL;
    }
    iter->arbol = arbol;
    iter->pila = pila_crear();
    if (!iter->pila) {
        abb_iter_in_destruir(iter);
        return NULL;
    }

    if (!_iter_apilar(iter->pila, arbol->raiz)) {
        abb_iter_in_destruir(iter);
        pila_destruir(iter->pila);
        return NULL;
    }

    return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter) {
    if (abb_iter_in_al_final(iter)) {
        return false;
    }
    nodo_t* nodo = (nodo_t*)pila_desapilar(iter->pila);
    if (nodo->der) {
        _iter_apilar(iter->pila, nodo->der);
    }
    return true; 
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
    void* tope = pila_ver_tope(iter->pila);
    return  tope? ((nodo_t*)(pila_ver_tope(iter->pila)))->clave : NULL;
}

bool abb_iter_in_al_final(const abb_iter_t *iter) {
    return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter) {
    pila_destruir(iter->pila);
    free(iter);
}


