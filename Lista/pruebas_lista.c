#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "testing.h"
#include "lista.h"

/*
cd /home/ian/Documentos/Lista
gcc -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -o pruebas *.c
valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./pruebas
*/

/* Inicio de pruebas */

/* Prueba1: lista vacia */
static void prueba_lista_vacia(void) {
    printf("PRUEBA 1 LISTA VACIA\n");
    lista_t *lista = lista_crear();
    print_test("Prueba1: crear lista vacia", lista != NULL);
    print_test("Prueba1: la lista esta vacia", lista_esta_vacia(lista));
    print_test("Prueba1: no hay elementos en la lista", lista_ver_primero(lista)==NULL && lista_ver_ultimo(lista)==NULL);
    lista_destruir(lista, NULL);
    printf("\n");
}

/* Prueba2: lista con NULL */
static void prueba_lista_con_NULL(void) {
    printf("PRUEBA 2 LISTA CON NULL\n");
    lista_t *lista = lista_crear();
    print_test("Prueba2: insertar NULL", lista_insertar_primero(lista, NULL)); 
    print_test("Prueba2: la lista no esta vacia", !lista_esta_vacia(lista));
    print_test("Prueba2: el primer elemento es NULL", lista_ver_primero(lista)==NULL);
    print_test("Prueba2: elimino el elemento", lista_borrar_primero(lista) == NULL);
    print_test("Prueba2: la lista esta vacia", lista_esta_vacia(lista));
    lista_destruir(lista, NULL);
    printf("\n");
}

//Prueba 3: cola con varios elementos
static void prueba_lista_varios_elementos(void) {
    printf("PRUEBA 3 LISTA CON POCOS ELEMENTOS\n");
    lista_t *lista = lista_crear();
    int numero1 = 1;
    char cadena[] = "Hola";
    char letra = 'a';
    //Inserto los tres elementos y veo el primer elemento en cada momento
    print_test("Prueba 3: inserto 1", lista_insertar_primero(lista, &numero1)); 
    print_test("Prueba 3: el primer elemento es 1", lista_ver_primero(lista)==&numero1);
    print_test("Prueba 3: El largo de la lista es correcto", lista_largo(lista) == 1);
    print_test("Prueba 3: inserto cadena", lista_insertar_primero(lista, &cadena));
    print_test("Prueba 3: el primer elemento es cadena", lista_ver_primero(lista)==&cadena);
    print_test("Prueba 3: El largo de la lista es correcto", lista_largo(lista) == 2);
    print_test("Prueba 3: inserto letra", lista_insertar_primero(lista, &letra));
    print_test("Prueba 3: el primer elemento es letra", lista_ver_primero(lista)==&letra);
    print_test("Prueba 3: El largo de la lista es correcto", lista_largo(lista) == 3);
    //Comienzo a borrar uno a uno viendo el primer elemento si la lista esta vacia
    print_test("Prueba 3: borro letra", lista_borrar_primero(lista) == &letra);
    print_test("Prueba 3: primer elemento es cadena", lista_ver_primero(lista)==&cadena);
    print_test("Prueba 3: la lista no esta vacia", !lista_esta_vacia(lista));
    print_test("Prueba 3: El largo de la lista es correcto", lista_largo(lista) == 2);
    print_test("Prueba 3: borro cadena", lista_borrar_primero(lista) == &cadena);
    print_test("Prueba 3: primer elemento es 1", lista_ver_primero(lista)==&numero1);
    print_test("Prueba 3: la lista no esta vacia", !lista_esta_vacia(lista));
    print_test("Prueba 3: El largo de la lista es correcto", lista_largo(lista) == 1);
    print_test("Prueba 3: borro 1", lista_borrar_primero(lista) == &numero1);
    print_test("Prueba 3: no hay elementos", lista_ver_primero(lista)==NULL);
    print_test("Prueba 3: la lista esta vacia", lista_esta_vacia(lista));
    print_test("Prueba 3: El largo de la lista es correcto", lista_largo(lista) == 0);
    //intento borrar en la lista vacia
    print_test("Prueba 3: no puedo borrar en lista vacia", lista_borrar_primero(lista) == NULL);
    //destruyo la lista
    lista_destruir(lista, NULL);
    printf("\n");
}

//Prueba 4: lista con varios intercalados
static void prueba_lista_volumen(size_t largo_vol) {
    printf("Prueba 4: PRUEBAS DE VOLUMEN\n");
    // Creo la lista y el arreglo que voy a enlistar
    int volumen[largo_vol];
    lista_t *lista = lista_crear(); 
    // inserto
    for (int i = 0; i < largo_vol; i++) {
        volumen[i] = i;
        // Solo llamo a print_test() dentro de los ifs
        // para no spammear la terminal 
        if (!lista_insertar_primero(lista, &volumen[i])) {
            print_test("Prueba 4: Error al insertar elementos", false);
            return;
        }
        if (lista_ver_primero(lista) != &volumen[i]) {
            print_test("Prueba 4: Error al ver primero, no se insertó correctamente el elemento", false);
            return;
        }
    }
    printf("Prueba 4: Prueba con %li elementos", largo_vol);
    print_test("",true);
    //Borro
    for(size_t i = 0; i < largo_vol; i++) {
        if (lista_ver_primero(lista) != &volumen[largo_vol - 1 - i]) {
            print_test("Prueba 4: Error al ver primero, no se borró correctamente el elemento", false);
            return;
        }
        if (lista_borrar_primero(lista) != &volumen[largo_vol - 1 - i]) {
            print_test("Prueba 4: Error al borrar el elemento", false);
            return;
        }
    }
    // Funciona como cuando estaba vacia
    print_test("Prueba 4: No se puede desapilar mas", !lista_borrar_primero(lista));
    print_test("Prueba 4: No se puede ver primero", !lista_ver_primero(lista));
    lista_destruir(lista, NULL);
} 

//Prueba 5: prueba basica iterador externo
static void prueba_iterar(void) {
    printf("\n");
    printf("Prueba 5: PRUEBA ITER EXTERNO BASICO\n");
    lista_t* lista = lista_crear();
    char* cadena = "Hola!";
    size_t largo = 5;
    for (size_t i = 0; i < largo; i++) {
        lista_insertar_ultimo(lista, &cadena[i]);
    }
    lista_iter_t* iter = lista_iter_crear(lista);
    size_t cont = 0;
    while (!lista_iter_al_final(iter)) {
        print_test("Prueba 5: El iter muestra el elemento correcto", *(char*)lista_iter_ver_actual(iter) == cadena[cont]);
        cont++;
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    lista_destruir(lista, NULL);
    printf("\n");
}

//Prueba 6: Iter insertar
static void prueba_iter_insertar(void) {
    printf("Prueba 6: PRUEBA ITER EXTERNO INSERTAR\n");
    lista_t* lista = lista_crear();
    int num1 = 1;
    int num2 = 2;
    int num3 = 3;
    int num4 = 4;
    int num5 = 5;
    lista_insertar_ultimo(lista, &num1);
    lista_insertar_ultimo(lista, &num2);
    lista_iter_t* iter = lista_iter_crear(lista);
    // 1 2
    print_test("Prueba 6: Se puede insertar al principio correctamente", lista_iter_insertar(iter, &num3));
    print_test("Prueba 6: El primer elemento es el correcto", lista_iter_ver_actual(iter) == &num3);
    print_test("Prueba 6: El largo de la lista es correcto", lista_largo(lista) == 3);
    // 3 1 2
    lista_iter_avanzar(iter);
    lista_iter_avanzar(iter);
    lista_iter_insertar(iter, &num4);
    print_test("Prueba 6: El elemento insertado es el correcto", lista_iter_ver_actual(iter) == &num4);
    print_test("Prueba 6: El largo de la lista es correcto", lista_largo(lista) == 4);
    // 3 1 4 2
    lista_iter_avanzar(iter);
    print_test("Prueba 6: Avanza bien", lista_iter_ver_actual(iter) == &num2);
    lista_iter_avanzar(iter);
    lista_iter_avanzar(iter);
    print_test("Prueba 6: Al llegar al final el elemento es NULL", lista_iter_ver_actual(iter) == NULL);
    lista_iter_insertar(iter, &num5);
    // 3 1 4 2 5
    print_test("Prueba 6: Se puede insertar al final correctamente", lista_iter_ver_actual(iter) == &num5);
    print_test("Prueba 6: El largo de la lista es correcto", lista_largo(lista) == 5);
    lista_iter_destruir(iter);
    lista_destruir(lista, NULL);
    printf("\n");
}

//Prueba 7: Insertar al final de ambas formas son equivalentes
static void prueba_iter_insertar_final(void) {
    printf("Prueba 7: PRUEBA ITER EXTERNO EQUIVALENCIA AL INSERTAR AL FINAL\n");
    lista_t* lista1 = lista_crear();
    lista_t* lista2 = lista_crear();
    int num1 = 1;
    int num2 = 2;
    int num3 = 3;
    lista_insertar_ultimo(lista1, &num1);
    lista_insertar_ultimo(lista1, &num2);
    lista_insertar_ultimo(lista2, &num1);
    lista_insertar_ultimo(lista2, &num2);
    // 1 2
    lista_iter_t* iter1 = lista_iter_crear(lista1);
    while (!lista_iter_al_final(iter1)){
            lista_iter_avanzar(iter1);
    }
    lista_iter_insertar(iter1, &num3);
    lista_iter_t* iter2 = lista_iter_crear(lista2);
    lista_insertar_ultimo(lista2, &num3);
    // 3 1 2
    print_test("Prueba 7: Los elementos insertados al final son correctos", lista_ver_ultimo(lista1) == lista_ver_ultimo(lista2));
    lista_iter_destruir(iter1);
    lista_destruir(lista1, NULL);
    lista_iter_destruir(iter2);
    lista_destruir(lista2, NULL);
    printf("\n");
}


//Prueba 8:  Al remover el elemento cuando se crea el iterador, cambia el primer elemento de la lista.
static void prueba_iter_borrar_primero(void) {
    printf("Prueba 8: PRUEBA ITER EXTERNO BORRAR PRIMERO\n");
    lista_t* lista = lista_crear();
    int num1 = 1;
    int num2 = 2;
    lista_insertar_ultimo(lista, &num1);
    lista_insertar_ultimo(lista, &num2);
    // 1 2
    lista_iter_t* iter = lista_iter_crear(lista);
    print_test("Prueba 8: el elemento devuelto es correcto correctos",     lista_iter_borrar(iter) == &num1);
    // 2
    print_test("Prueba 8: El elemento borrado fue correcto", lista_iter_ver_actual(iter) == &num2);
    lista_iter_destruir(iter);
    lista_destruir(lista, NULL);
    printf("\n");
}

//Prueba 9:  Remover elemento del medio.
static void prueba_iter_borrar_medio(void) {
    printf("Prueba 9: PRUEBA ITER EXTERNO BORRAR CENTRAL\n");
    lista_t* lista = lista_crear();
    int num1 = 1;
    int num2 = 2;
    int num3 = 3;
    lista_insertar_ultimo(lista, &num1);
    lista_insertar_ultimo(lista, &num2);
    lista_insertar_ultimo(lista, &num3);
    // 1 2 3
    lista_iter_t* iter1 = lista_iter_crear(lista);
    lista_iter_t* iter2 = lista_iter_crear(lista);
    lista_iter_avanzar(iter1);
    print_test("Prueba 9: El elemento devuelto es correcto", lista_iter_borrar(iter1) == &num2);
    // 1 3
    print_test("Prueba 9: El primer elemento es 1", lista_iter_ver_actual(iter2) == &num1);
    lista_iter_avanzar(iter2);
    print_test("Prueba 9: El segundo elemento es 3", lista_iter_ver_actual(iter2) == &num3);
    lista_iter_avanzar(iter2);
    print_test("Prueba 9: Se llegó al final de la lista", lista_iter_ver_actual(iter2) == NULL);
    lista_iter_destruir(iter1);
    lista_iter_destruir(iter2);
    lista_destruir(lista, NULL);
    printf("\n");
}


//Prueba 10:  Remover elemento del final.
static void prueba_iter_borrar_final(void) {
    printf("Prueba 10: PRUEBA ITER EXTERNO BORRAR FINAL\n");
    lista_t* lista = lista_crear();
    int num1 = 1;
    int num2 = 2;
    int num3 = 3;
    lista_insertar_ultimo(lista, &num1);
    lista_insertar_ultimo(lista, &num2);
    lista_insertar_ultimo(lista, &num3);
    // 1 2 3
    lista_iter_t* iter = lista_iter_crear(lista);
    lista_iter_avanzar(iter);
    lista_iter_avanzar(iter);
    printf("Prueba 10: El elemento a borrar es: %d \n", *(int*)lista_iter_ver_actual(iter));
    print_test("Prueba 10: el elemento devuelto es correcto", lista_iter_borrar(iter) == &num3);
    // 1 2
    print_test("Prueba 10: el primer elemento es 1", lista_ver_primero(lista) == &num1);
    print_test("Prueba 10: El ultimo elemento es 2", lista_ver_ultimo(lista) == &num2);
    lista_iter_destruir(iter);
    lista_destruir(lista, NULL);
    printf("\n");
}

//Prueba 11:  Prueba con corte del iterador interno
bool suma_primeros_2_elementos(void* dato, void* extra) {
    int* extras = (int*)extra;  
    if (extras[0] == extras[1]) {
        return false;
    }
    extras[0]++;
    extras[2] += *(int*)dato;
    return true;
}

static void prueba_iter_interno_con_corte(void) {
    printf("Prueba 11: ITERADOR INTERNO CON CORTE\n");
    lista_t* lista = lista_crear();
    int num1 = 1;
    int num2 = 2;
    int num3 = 3;
    lista_insertar_ultimo(lista, &num1);
    lista_insertar_ultimo(lista, &num2);
    lista_insertar_ultimo(lista, &num3);
    // 1 2 3
    int extra[3];
    extra[0] = 0; //cantidad de veces que se ejecutó
    extra[1] = 2; //cantidad de elementos de la lista a sumar
    extra[2] = 0; //sumado hasta el momento
    lista_iterar(lista, suma_primeros_2_elementos, &extra);
    print_test("Prueba 11: La funcion suma correctamente los primeros n elementos", extra[2] == num1 + num2);
    lista_destruir(lista, NULL);
    printf("\n");
}

//Prueba 12:  Prueba sin corte del iterador interno
bool suma_todos_los_elementos(void* dato, void* extra) {
    *(int*)extra += *(int*)dato;
    return true;
}

static void prueba_iter_interno_sin_corte(void) {
    printf("Prueba 12: ITERADOR INTERNO SIN CORTE\n");
    lista_t* lista = lista_crear();
    int num1 = 1;
    int num2 = 2;
    int num3 = 3;
    lista_insertar_ultimo(lista, &num1);
    lista_insertar_ultimo(lista, &num2);
    lista_insertar_ultimo(lista, &num3);
    // 1 2 3
    int extra = 0; //elementos sumados
    lista_iterar(lista, suma_todos_los_elementos, &extra);
    print_test("Prueba 12: La funcion suma correctamente todos los", extra == num1 + num2 + num3);
    lista_destruir(lista, NULL);
    printf("\n");
}

//Prueba 13:  Destruir lista y sus elementos internos
typedef struct puerta {
    bool abierta;
} puerta_t;

puerta_t* puerta_crear() {
    puerta_t* puerta = malloc(sizeof(puerta_t));
    if (puerta == NULL){
        return NULL;
    }
    puerta->abierta = false;
    return puerta;
}

void puerta_destruir(void* puerta) {
    free(puerta);
}

static void prueba_destruccion_free(void) {
    printf("Prueba 13: DESTRUIR LISTA CON FUNCION FREE\n");
    lista_t* lista = lista_crear();
    puerta_t* puerta1 = puerta_crear();
    puerta_t* puerta2 = puerta_crear();
    puerta_t* puerta3 = puerta_crear();
    lista_insertar_ultimo(lista, puerta1);
    print_test("Prueba 13: Se inserto correctamente el elemento", lista_ver_ultimo(lista) == puerta1);
    lista_insertar_ultimo(lista, puerta2);
    print_test("Prueba 13: Se inserto correctamente el elemento", lista_ver_ultimo(lista) == puerta2);
    lista_insertar_ultimo(lista, puerta3);
    print_test("Prueba 13: Se inserto correctamente el elemento", lista_ver_ultimo(lista) == puerta3);
    lista_destruir(lista, free);
    printf("\n");
}

//Prueba 14:  Destruir lista sin destruir sus elementos internos
static void prueba_destruccion_NULL(void) {
    printf("Prueba 14: DESTRUIR LISTA SIN FUNCION DE DESTRUIR\n");
    lista_t* lista_padre = lista_crear();
    lista_t* lista1 = lista_crear();
    lista_t* lista2 = lista_crear();
    lista_t* lista3 = lista_crear();
    lista_insertar_ultimo(lista_padre, lista1);
    print_test("Puerta 14: Se inserto correctamente el elemento", lista_ver_ultimo(lista_padre) == lista1);
    lista_insertar_ultimo(lista_padre, lista2);
    print_test("Puerta 14: Se inserto correctamente el elemento", lista_ver_ultimo(lista_padre) == lista2);
    lista_insertar_ultimo(lista_padre, lista3);
    print_test("Puerta 14: Se inserto correctamente el elemento", lista_ver_ultimo(lista_padre) == lista3);
    lista_destruir(lista_padre, NULL);
    // Si no eliminamos las otras listas a mano, se perderia memoria
    print_test("Puerta 14: Los elementos aun existen", lista_esta_vacia(lista1) && lista_esta_vacia(lista2) && lista_esta_vacia(lista3));
    lista_destruir(lista1, NULL);
    lista_destruir(lista2, NULL);
    lista_destruir(lista3, NULL);
    printf("\n");
}

//Prueba 15:  Destruir lista y sus elementos internos
static void prueba_destruccion_otra_funcion(void) {
    printf("Prueba 15: DESTRUIR LISTA CON OTRA FUNCION\n");
    lista_t* lista = lista_crear();
    puerta_t* puerta1 = puerta_crear();
    puerta_t* puerta2 = puerta_crear();
    puerta_t* puerta3 = puerta_crear();
    lista_insertar_ultimo(lista, puerta1);
    print_test("Puerta 15: Se inserto correctamente el elemento", lista_ver_ultimo(lista) == puerta1);
    lista_insertar_ultimo(lista, puerta2);
    print_test("Puerta 15: Se inserto correctamente el elemento", lista_ver_ultimo(lista) == puerta2);
    lista_insertar_ultimo(lista, puerta3);
    print_test("Puerta 15: Se inserto correctamente el elemento", lista_ver_ultimo(lista) == puerta3);
    lista_destruir(lista, puerta_destruir);
    printf("\n");
}

//Prueba 16:  Destruir lista vacia y sus elementos internos
static void prueba_destruccion_vacia_free(void) {
    printf("Prueba 16: DESTRUIR LISTA VACIA CON FUNCION FREE\n");
    lista_t* lista = lista_crear();
    print_test("Prueba 16: La lista esta vacia", lista_esta_vacia(lista));
    lista_destruir(lista, free);
    printf("\n");
}

//Prueba 17:  Destruir lista vacia
static void prueba_destruccion_vacia_NULL(void) {
    printf("Prueba 17: DESTRUIR LISTA VACIA SIN FUNCION DE DESTRUIR\n");
    lista_t* lista = lista_crear();
    print_test("Prueba 17: La lista esta vacia", lista_esta_vacia(lista));
    lista_destruir(lista, NULL);
}

void pruebas_lista_estudiante() {
    // PRUEBAS LISTA
    prueba_lista_vacia();
    prueba_lista_con_NULL();
    prueba_lista_varios_elementos();
    prueba_lista_volumen(100);
    prueba_lista_volumen(1000);
    prueba_lista_volumen(10000);

    // PRUEBAS ITER EXTERNO
    prueba_iterar();
    prueba_iter_insertar();
    prueba_iter_insertar_final();
    prueba_iter_borrar_primero();
    prueba_iter_borrar_medio();
    prueba_iter_borrar_final();

    // PRUEBAS ITER INTERNO
    prueba_iter_interno_con_corte();
    prueba_iter_interno_sin_corte();

    // PRUEBAS DESTRUCCION
    prueba_destruccion_free(); 
    prueba_destruccion_NULL();
    prueba_destruccion_otra_funcion();
    prueba_destruccion_vacia_free(); 
    prueba_destruccion_vacia_NULL();
}

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_lista_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif