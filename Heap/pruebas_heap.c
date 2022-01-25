
#include "heap.h"
#include "pila.h"
#include "testing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For ssize_t in Linux.
#include <time.h>

/*
gcc -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -o pruebas *.c
valgreen --leak-check=full --track-origins=yes --show-reachable=yes ./pruebas
*/

/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * ****************************************************************** */

int comparar_cadenas (const void* a, const void* b) {
    return strcmp((char*)a, (char*)b);
}

int comparar_numeros (const void* a, const void* b) {
    if (*(int*)a == *(int*)b) {
        return 0;
    }
    if (*(int*)a > *(int*)b){
        return 1;
    }
    return -1;
}

static void prueba_crear_heap_vacio() {
    heap_t* heap = heap_crear(comparar_numeros);

    print_test("Prueba heap crear heap vacio", heap);
    print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);
    print_test("Prueba heap obtener maximo, es NULL, no existe", !heap_ver_max(heap));
    print_test("Prueba heap desencolar, es NULL, no se puede", !heap_desencolar(heap));
    print_test("Prueba heap esta vacio, es true", heap_esta_vacio(heap));

    heap_destruir(heap, NULL);
}

static void prueba_heap_insertar() {
    heap_t* heap = heap_crear(comparar_numeros);

    int n1 = 1;
    int n2 = 2;
    int n3 = 3;

    // Inserta 1 valor y luego lo borra
    print_test("Prueba heap insertar n1", heap_encolar(heap, &n1));
    print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);
    print_test("Prueba heap obtener maximo es n1", heap_ver_max(heap) == &n1);
    print_test("Prueba heap desencolar n1", heap_desencolar(heap) == &n1);
    print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);
    
    // Inserta otros 2 valores y no los borra (se destruyen con el heap)
    print_test("Prueba heap insertar n2", heap_encolar(heap, &n2));
    print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);
    print_test("Prueba heap obtener maximo es n2", heap_ver_max(heap) == &n2);

    print_test("Prueba heap insertar n3", heap_encolar(heap, &n3));
    print_test("Prueba heap la cantidad de elementos es 2", heap_cantidad(heap) == 2);
    print_test("Prueba heap obtener maximo es n3", heap_ver_max(heap) == &n3);

    heap_destruir(heap, NULL);
}


int comparar_pilas_int(const void* p1, const void* p2) {
    void* void_tope1 = pila_ver_tope(p1);
    void* void_tope2 = pila_ver_tope(p2);
    int tope1 = void_tope1 ? *(int*)void_tope1 : 0;
    int tope2 = void_tope2 ? *(int*)void_tope2 : 0;

    if (tope1 == tope2) {
        return 0;
    }
    return tope1 > tope2 ? 1 : -1;
}

void _pila_destruir(void* p) {
    pila_destruir(p);
}

static void prueba_heap_desencolar() {
    heap_t* heap = heap_crear(comparar_numeros);
    
    int n1 = 1;
    int n2 = 2;
    int n3 = 3;

    // Inserta 3 valores y luego los borra
    print_test("Prueba heap insertar n1", heap_encolar(heap, &n1));
    print_test("Prueba heap insertar n2", heap_encolar(heap, &n2));
    print_test("Prueba heap insertar n3", heap_encolar(heap, &n3));

    // Al borrar cada elemento comprueba que ya no está pero los otros sí.
    print_test("Prueba heap el maximo es el correcto", heap_ver_max(heap) == &n3);
    print_test("Prueba heap borrar n3", heap_desencolar(heap) == &n3);
    print_test("Prueba heap el maximo es el correcto", heap_ver_max(heap) == &n2);
    print_test("Prueba heap la cantidad de elementos es 2", heap_cantidad(heap) == 2);

    print_test("Prueba heap borrar n2", heap_desencolar(heap) == &n2);
    print_test("Prueba heap el maximo es el correcto", heap_ver_max(heap) == &n1);
    print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);

    print_test("Prueba heap borrar n1", heap_desencolar(heap) == &n1);
    print_test("Prueba heap no se puede ver maximo", !heap_ver_max(heap));
    print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);

    heap_destruir(heap, NULL);
}

static void prueba_heap_orden_correcto() {
    heap_t* heap = heap_crear(comparar_numeros);

    int n1 = 1;
    int n2 = 2;
    int n3 = 3;
    int n4 = 4;
    int n5 = 5;
    bool ok = true;

    print_test("Prueba heap insertar n1", heap_encolar(heap, &n1));
    print_test("Prueba heap insertar n2", heap_encolar(heap, &n2));
    print_test("Prueba heap insertar n3", heap_encolar(heap, &n3));
    print_test("Prueba heap insertar n4", heap_encolar(heap, &n4));
    print_test("Prueba heap insertar n5", heap_encolar(heap, &n5));

    while (!heap_esta_vacio(heap)) {
        void* max_ant = heap_desencolar(heap);
        void* max_act = heap_ver_max(heap);
        if (!max_act) {
            break;
        }
        if (*(int*)max_ant < *(int*)max_act) {
            ok = false;
            break;
        }
    }
    print_test("Prueba heap los elementos estaban en el orden correcto", ok);
    print_test("Prueba heap la cantidad de elementos es correcta", heap_cantidad(heap) == 0);
    heap_destruir(heap, NULL);
}

static void prueba_heap_desde_arreglo() {
    int n1 = 1;
    int n3 = 3;
    int n2 = 2;
    int n7 = 7;
    int n6 = 6;
    int n8 = 8;
    int n9 = 9;
    void* arr[] = {&n9, &n3, &n2, &n7, &n6, &n8, &n1};
    size_t cant = 7;
        
    heap_t* heap = heap_crear_arr(arr, cant, comparar_numeros);

    print_test("Prueba heap la cantidad de elementos es correcta", heap_cantidad(heap) == cant);

    bool ok = true;
    while (!heap_esta_vacio(heap)) {
        void* max_ant = heap_desencolar(heap);
        void* max_act = heap_ver_max(heap);
        if (!max_act) {
            break;
        }
        if (comparar_numeros(max_ant, max_act) < 0) {
            ok = false;
            break;
        }
    }
    print_test("Prueba heap los elementos estaban en el orden correcto", ok);
    print_test("Prueba heap la cantidad de elementos es correcta", heap_cantidad(heap) == 0);

    heap_destruir(heap, NULL);
}

bool _arr_ordenado(void* arr[], size_t cant) {
    for (size_t i = 1; i < cant; i++) {
        if (comparar_numeros(arr[i], arr[i-1]) < 0) {
        return false;
        }
    }
    return true;
}

static void prueba_heapsort() {
    int n1 = 1;
    int n3 = 3;
    int n2 = 2;
    int n7 = 7;
    int n6 = 6;
    int n8 = 8;
    int n9 = 9;
    void* arr[] = {&n9, &n3, &n2, &n7, &n6, &n8, &n1};
    size_t cant = 7;
    
    print_test("El arreglo esta ordenado es false", !_arr_ordenado(arr, cant));
    heap_sort(arr, cant, comparar_numeros);
    print_test("El arreglo esta ordenado es true", _arr_ordenado(arr, cant));
}

static void prueba_heapsort_volumen(size_t largo) {
    void* arr[largo];
    
    for (size_t i = 0; i < largo; i++) {
        int* elem = malloc(sizeof(int));
        *elem = (int)i + (rand() % (int)largo);
        arr[i] = elem;
    }

    print_test("El arreglo NO esta ordenado", !_arr_ordenado(arr, largo));

    heap_sort(arr, largo, comparar_numeros);

    print_test("El arreglo esta ordenado", _arr_ordenado(arr, largo));

    for (size_t i = 0; i < largo; i++) {
        free(arr[i]);
    }
}

static void prueba_heap_volumen(size_t largo, bool debug) {
    heap_t* heap = heap_crear(comparar_pilas_int);

    bool ok = true;
    //guardamos en el heap pilas
    for (unsigned i = 0; i < largo; i++) {
        pila_t* pila = pila_crear();
        ok = heap_encolar(heap, pila);
        if (!ok) {
            break;
        }
    }

    if (debug) print_test("Prueba heap almacenar muchos elementos", ok);
    if (debug) print_test("Prueba heap la cantidad de elementos es correcta", heap_cantidad(heap) == largo);

    heap_destruir(heap, _pila_destruir);
}

/* ******************************************************************
 *                        FUNCIÓN PRINCIPAL
 * ******************************************************************* */
void pruebas_heap_estudiante()
{
    clock_t start = clock();

    // Ejecuta todas las pruebas unitarias.
    prueba_crear_heap_vacio();
    prueba_heap_insertar();
    prueba_heap_desencolar();
    prueba_heap_orden_correcto();
    prueba_heap_desde_arreglo();
    prueba_heapsort();
    prueba_heapsort_volumen(5000); 
    prueba_heap_volumen(50000, true);
    prueba_heap_volumen(50000, true);
    clock_t end = clock();
    double elapsed = (double)((double)end - (double)start)/CLOCKS_PER_SEC;
    printf("Time measured: %.3f seconds.\n", elapsed);
}

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_heap_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif



