#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "heap.h"

/*
gcc -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -o pruebas *.c
valgreen --leak-check=full --track-origins=yes --show-reachable=yes ./pruebas
*/

#define TAMANIO_INICIAL 7
#define FACTOR_REDIMENSION_AGRANDAR 2.0
#define FACTOR_REDIMENSION_ACHICAR 0.5
#define CRITERIO_REDIMENSION_ACHICAR 0.1

struct heap {
    void** datos;
    size_t cant;
    size_t tam;
    cmp_func_t cmp;
};

/*
    Recibe un arreglo, su largo y una funcion de comparacion y lo
    convierte en heap in-place.
*/
void _heapify(void *elementos[], size_t cant, cmp_func_t cmp);

/* 
    Intercambia dos valores void*.
*/
void _swap(void **x, void **y);
/*
    Recibe un arreglo, una posicion y una funcion de comparacion y
    deja con forma de heap desde la posicion que recibe hasta la 
    primer posicion del arreglo.
*/
void _upheap(void* arr[], size_t pos, cmp_func_t cmp);
/*
    Recibe un arreglo, una posiciones del mismo y una funcion de comparacion y
    devuelve la posicion que tiene el máximo valor entre esa posicion y sus hijos
*/
size_t _maximo(void* arr[], cmp_func_t cmp, size_t padre, size_t cant);
/*
    Recibe un arreglo, el largo, una posicion y una funcion de comparacion y
    deja con forma de heap desde la posicion que recibe hasta la 
    ultima posicion del arreglo.
*/
void _downheap(void* arr[], size_t cant, size_t pos, cmp_func_t cmp);

/*
    Retorna true si el arreglo cumple con la condicion de heap,
    false en caso contrario.
*/
bool _es_heap(const void* arr[], size_t cant, size_t pos_padre, size_t pos_hijo, cmp_func_t cmp);

/*
    Cambia el tamaño del heap. Devuelve false en caso de error. True
    si se modifico el tamaño correctamente.
*/
bool _heap_redimensionar(heap_t* heap, double factor_redimension);


void _heapify(void *elementos[], size_t cant, cmp_func_t cmp) {
    for (int i = (int)cant - 1; i > -1; i--) {
        _downheap(elementos, cant, (size_t)i, cmp);
    }
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) {
    _heapify(elementos, cant, cmp);
    
    for (size_t i = (cant - 1); i > 0; i--) {
        _swap(&elementos[i],&elementos[0]);
        _downheap(elementos, i, 0, cmp);
    }
}

heap_t *heap_crear(cmp_func_t cmp) {
    heap_t* heap = calloc(1, sizeof(heap_t));
    if (!heap) {
        return NULL;
    }
    heap->tam = TAMANIO_INICIAL;

    heap->datos = calloc(1, heap->tam * sizeof(void*));
    if (!heap->datos) {
        free(heap);
        return NULL;
    }
    heap->cmp = cmp;
    return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
    heap_t* heap = heap_crear(cmp);
    if (!heap) {
        return NULL;
    }

    while (n >= heap->tam) {
        _heap_redimensionar(heap, FACTOR_REDIMENSION_AGRANDAR);
    }

    for (size_t i = 0; i < n; i++) {
        heap->datos [i] = arreglo[i];
    }

    heap->cant = n;
    heap->cmp = cmp;

    _heapify(heap->datos, heap->cant, heap->cmp);

    return heap;
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)) {
    if (destruir_elemento) {
        for (size_t i = 0; i < heap->cant; i++) {
            destruir_elemento(heap->datos[i]);
        }
    }
    free(heap->datos);
    free(heap);
}

size_t heap_cantidad(const heap_t *heap) {
    return heap->cant;
}

bool heap_esta_vacio(const heap_t *heap) {
    return heap->cant == 0;
}

void _swap(void **x, void **y) {
    void* aux = *x;
    *x = *y;
    *y = aux;
}

void _upheap(void* arr[], size_t pos, cmp_func_t cmp) {
    if (!pos) {
        return;
    }
    size_t pos_padre = (pos - 1) / 2;

    if (cmp(arr[pos_padre], arr[pos]) > 0) {
        return;
    }
    _swap(&arr[pos_padre], &arr[pos]);
    _upheap(arr, pos_padre, cmp);
}

bool _heap_redimensionar(heap_t* heap, double factor_redimension) {
    size_t nuevo_tamanio = (size_t)((double)heap->tam*factor_redimension);
    if (nuevo_tamanio <= TAMANIO_INICIAL) {
        return false;
    }
    void** tabla_nueva = realloc(heap->datos, nuevo_tamanio * sizeof(void*));
    if (!tabla_nueva) {
        return false;
    }
    heap->tam = nuevo_tamanio;
    heap->datos = tabla_nueva;

    return true;
}

bool heap_encolar(heap_t *heap, void *elem) {
    if (heap->cant == heap->tam) {
        if (!_heap_redimensionar(heap, FACTOR_REDIMENSION_AGRANDAR)){
            return false;
        }
    }
    heap->datos[heap->cant] = elem;    
    _upheap(heap->datos, heap->cant, heap->cmp);
    heap->cant++;

    return true;
}

void *heap_ver_max(const heap_t *heap) {
    if (heap_esta_vacio(heap)) {
        return NULL;
    }
    return heap->datos[0];
}

size_t _maximo(void* arr[], cmp_func_t cmp, size_t padre, size_t cant) {
    size_t h_izq = (2 * padre) + 1;
    size_t h_der = (2 * padre) + 2;
    size_t max;
    max = cmp(arr[padre], arr[h_izq]) >= 0 ? padre : h_izq;
    if (h_der < cant) {
        max = cmp(arr[max], arr[h_der]) >= 0 ? max : h_der;
    }
    return max;
}

void _downheap(void* arr[], size_t cant, size_t pos, cmp_func_t cmp) {
    size_t pos_izq = (2 * pos) + 1;
    if (pos_izq >= cant) {
        return; 
    }
    
    size_t maximo = _maximo(arr, cmp, pos, cant);
    if (maximo != pos) {
        _swap(&arr[pos], &arr[maximo]);
        _downheap(arr, cant, maximo, cmp);
    }
}

void *heap_desencolar(heap_t *heap) {
    if (heap_esta_vacio(heap)) {
        return NULL;
    }
    void* dato = heap->datos[0];
    if (heap->cant == 1) {
        heap->cant--;
        return dato;
    }

    _swap(&heap->datos[0], &heap->datos[heap->cant-1]);
    heap->cant--;

    _downheap(heap->datos, heap->cant, 0, heap->cmp);

    if ((double)heap->cant / (double)heap->tam <= CRITERIO_REDIMENSION_ACHICAR) {
        _heap_redimensionar(heap, FACTOR_REDIMENSION_ACHICAR);
    }

    return dato;
}
