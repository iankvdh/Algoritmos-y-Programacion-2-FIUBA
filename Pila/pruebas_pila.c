#include "pila.h"
#include "testing.h"
#include <stddef.h>

#include <stdio.h>



/* Inicio de pruebas */

/* Prueba1: pila vacia */
static void prueba_pila_vacia(void) {
    pila_t *pila1 = pila_crear();
    print_test("Prueba1: crear pila vacia", pila1 != NULL);
    print_test("Prueba1: la pila esta vacia", pila_esta_vacia(pila1));
    print_test("Prueba1: No hay tope", pila_ver_tope(pila1)==NULL);
    pila_destruir(pila1);
}

/* Prueba2: pila con un solo elemento */
static void prueba_pila_un_elemento(void) {
    pila_t *pila2 = pila_crear();
    int numero15 = 15;
    print_test("Prueba2: apilar 15", pila_apilar(pila2, &numero15)); 
    print_test("Prueba2: la pila no esta vacia", !pila_esta_vacia(pila2));
    print_test("Prueba2: El tope es 15", pila_ver_tope(pila2)==&numero15);
    print_test("Prueba2: despilo el 15", pila_desapilar(pila2) == &numero15);
    print_test("Prueba2: la pila esta vacia", pila_esta_vacia(pila2));
    print_test("Prueba2: No hay tope", pila_ver_tope(pila2)==NULL);
    //repito el proceso con otro numero
    int numero16 = 16;
    print_test("Prueba2: apilar 16", pila_apilar(pila2, &numero16)); 
    print_test("Prueba2: la pila no esta vacia", !pila_esta_vacia(pila2));
    print_test("Prueba2: El tope es 16", pila_ver_tope(pila2)==&numero16);
    print_test("Prueba2: despilo el 16", pila_desapilar(pila2) == &numero16);
    print_test("Prueba2: la pila esta vacia", pila_esta_vacia(pila2));
    print_test("Prueba2: No hay tope", pila_ver_tope(pila2)==NULL);
    //destruyo la pila
    pila_destruir(pila2);
}

/* Prueba3: pila con varios elementos */
static void prueba_pila_varios_elementos(void) {
    pila_t *pila3 = pila_crear();
    int numero1 = 1;
    char cadena[] = "Hola";
    char letra = 'a';
    //Apilo los tres elementos y veo el tope en cada momento
    print_test("Prueba3: apilar 1", pila_apilar(pila3, &numero1)); 
    print_test("Prueba3: el tope es 1", pila_ver_tope(pila3)==&numero1);
    print_test("Prueba3: apilar cadena", pila_apilar(pila3, &cadena));
    print_test("Prueba3: el tope es cadena", pila_ver_tope(pila3)==&cadena);
    print_test("Prueba3: apilar letra", pila_apilar(pila3, &letra));
    print_test("Prueba3: l tope es letra", pila_ver_tope(pila3)==&letra);
    //Comienzo a desapilar uno a uno viendo el tope y corroborando si la pila esta vacia
    print_test("Prueba3: despilo letra", pila_desapilar(pila3) == &letra);
    print_test("Prueba3: el tope es cadena", pila_ver_tope(pila3)==&cadena);
    print_test("Prueba3: la pila no esta vacia", !pila_esta_vacia(pila3));
    print_test("Prueba3: despilo cadena", pila_desapilar(pila3) == &cadena);
    print_test("Prueba3: el tope es 1", pila_ver_tope(pila3)==&numero1);
    print_test("Prueba3: la pila no esta vacia", !pila_esta_vacia(pila3));
    print_test("Prueba3: despilo 1", pila_desapilar(pila3) == &numero1);
    print_test("Prueba3: No hay tope", pila_ver_tope(pila3)==NULL);
    print_test("Prueba3: la pila esta vacia", pila_esta_vacia(pila3));
    //intento desapilar en la pila vacia
    print_test("Prueba3: despilo en pila vacia", pila_desapilar(pila3) == NULL);
    //destruyo la pila
    pila_destruir(pila3);
}

/* Prueba4: apilo, apilo, desapilo y vuelvo a apilar */
static void prueba_pila_apilar_desapilar(void) {
    pila_t *pila4 = pila_crear();
    int numero2 = 2;
    char cadena[] = "Chau";
    char letra = 'b';
    //Apilo dos elementos y veo el tope en cada momento
    print_test("Prueba4: apilar 2", pila_apilar(pila4, &numero2)); 
    print_test("Prueba4: el tope es 2", pila_ver_tope(pila4)==&numero2);
    print_test("Prueba4: apilar cadena", pila_apilar(pila4, &cadena));
    print_test("Prueba4: el tope es cadena", pila_ver_tope(pila4)==&cadena);
    print_test("Prueba4: despilo cadena", pila_desapilar(pila4) == &cadena);
    print_test("Prueba4: el tope es 2", pila_ver_tope(pila4)==&numero2);
    print_test("Prueba4: la pila no esta vacia", !pila_esta_vacia(pila4));
    print_test("Prueba4: apilar letra", pila_apilar(pila4, &letra)); 
    print_test("Prueba4: el tope es letra", pila_ver_tope(pila4)==&letra);
    print_test("Prueba4: despilo letra", pila_desapilar(pila4) == &letra);
    print_test("Prueba4: el tope es 2", pila_ver_tope(pila4)==&numero2);
    print_test("Prueba4: la pila no esta vacia", !pila_esta_vacia(pila4));
    print_test("Prueba4: despilo 2", pila_desapilar(pila4) == &numero2);
    print_test("Prueba4: No hay tope", pila_ver_tope(pila4)==NULL);
    print_test("Prueba4: la pila esta vacia", pila_esta_vacia(pila4));
    //intento desapilar en la pila vacia
    print_test("Prueba4: despilo en pila vacia", pila_desapilar(pila4) == NULL);
    //destruyo la pila
    pila_destruir(pila4);
}

/* Prueba5: Prueba de volumen (11 elementos) */
static void prueba_pila_volumen(void) {
    pila_t *pila5 = pila_crear();
    int cantidad_elemntos = 11;
    int vector_prueba[cantidad_elemntos];

    for (int i =0; i<cantidad_elemntos; i++) {
        vector_prueba[i] = i;
        print_test("Prueba5: apilar elemento i", pila_apilar(pila5, &vector_prueba[i])); 
        print_test("Prueba5: el tope es i", pila_ver_tope(pila5)==&vector_prueba[i]);
    }
    for (int j =cantidad_elemntos-1; j>-1; j--) {
        print_test("Prueba5: desapilo elemento j", pila_desapilar(pila5) == &vector_prueba[j]);
        if (j != -0) {
            print_test("Prueba5: el tope es elemento j-1", pila_ver_tope(pila5)==&vector_prueba[j-1]);
        }
        else {
            print_test("Prueba5: no hay tope", pila_ver_tope(pila5)==NULL);
            print_test("Prueba5: la pila esta vacia", pila_esta_vacia(pila5));
        }
    }
    //intento desapilar en la pila vacia
    print_test("Prueba5: despilo en pila vacia", pila_desapilar(pila5) == NULL);
    //destruyo la pila
    pila_destruir(pila5);
}

/* Prueba6: apilo un elemento NULL */
static void prueba_pila_apilar_null(void) {
    pila_t *pila6 = pila_crear();
        print_test("Prueba6: apilar elemento nulo", pila_apilar(pila6, NULL)); 
        print_test("Prueba6: el tope es NULL", pila_ver_tope(pila6)==NULL);
        print_test("Prueba6: la pila no esta vacia", !pila_esta_vacia(pila6));
        print_test("Prueba6: desapilo NULL", pila_desapilar(pila6) == NULL);
        print_test("Prueba6: la pila esta vacia", pila_esta_vacia(pila6));
    //destruyo la pila
    pila_destruir(pila6);
}

/* Prueba7: comprobar que al desapilar todos los elementos la capacidad de la pila sea igual a la de una recien creada */
static void prueba_pila_capacidad_inicial(void) {
    pila_t *pila7 = pila_crear();
    pila_t *pila8 = pila_crear();
    char cadena[] = "CADENA";
    //Apilo 20 elementos
    for (int i =0; i<20; i++) {
        pila_apilar(pila7, &cadena);
    }
    for (int j =0; j<20; j++) {
            pila_desapilar(pila7);
    }
    print_test("Prueba7: una pila vacia es como una pila recien creada 1", pila_desapilar(pila7) == pila_desapilar(pila8));
    print_test("Prueba7: una pila vacia es como una pila recien creada 2", pila_esta_vacia(pila7) == pila_esta_vacia(pila8));
    //destruyo las pilas
    pila_destruir(pila7);
    pila_destruir(pila8);
}

void pruebas_pila_estudiante() {
    prueba_pila_vacia();
    // ...
    prueba_pila_un_elemento();
    prueba_pila_varios_elementos();
    prueba_pila_apilar_desapilar();
    prueba_pila_volumen();
    prueba_pila_apilar_null();
    prueba_pila_capacidad_inicial();
}

/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_pila_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
