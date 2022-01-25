#include "cola.h"
#include "testing.h"
#include <stddef.h>

#include <stdlib.h>
#include "pila.h"


/* Inicio de pruebas */

/* Prueba1: cola vacia */
static void prueba_cola_vacia(void) {
    cola_t *cola1 = cola_crear();
    print_test("Prueba1: crear cola vacia", cola1 != NULL);
    print_test("Prueba1: la cola esta vacia", cola_esta_vacia(cola1));
    print_test("Prueba1: No hay elementos encolados", cola_ver_primero(cola1)==NULL);
    cola_destruir(cola1, NULL);
}

/* Prueba2: cola con un solo elemento */
static void prueba_cola_un_elemento(void) {
    cola_t *cola2 = cola_crear();
    int numero15 = 15; 
    print_test("Prueba2: encolar 15", cola_encolar(cola2, &numero15)); 
    print_test("Prueba2: la cola no esta vacia", !cola_esta_vacia(cola2));
    print_test("Prueba2: primer elemento es 15", cola_ver_primero(cola2)==&numero15);
    print_test("Prueba2: desencolo el 15", cola_desencolar(cola2) == &numero15);
    print_test("Prueba2: la cola esta vacia", cola_esta_vacia(cola2));
    print_test("Prueba2: no hay elementos", cola_ver_primero(cola2)==NULL);
    //repito el proceso con otro numero
    int numero16 = 16;
    print_test("Prueba2: encolar 16", cola_encolar(cola2, &numero16)); 
    print_test("Prueba2: la cola no esta vacia", !cola_esta_vacia(cola2));
    print_test("Prueba2: primer elemento es 16", cola_ver_primero(cola2)==&numero16);
    print_test("Prueba2: desencolo el 16", cola_desencolar(cola2) == &numero16);
    print_test("Prueba2: la cola esta vacia", cola_esta_vacia(cola2));
    print_test("Prueba2: no hay elementos", cola_ver_primero(cola2)==NULL);
    //destruyo la cola
    cola_destruir(cola2, NULL);
}

//Prueba3: cola con varios elementos
static void prueba_cola_varios_elementos(void) {
    cola_t *cola3 = cola_crear();
    int numero1 = 1;
    char cadena[] = "Hola";
    char letra = 'a';
    //Encolo los tres elementos y veo el primer elemento en cada momento
    print_test("Prueba3: encolar 1", cola_encolar(cola3, &numero1)); 
    print_test("Prueba3: primer elem es 1", cola_ver_primero(cola3)==&numero1);
    print_test("Prueba3: encolar cadena", cola_encolar(cola3, &cadena));
    print_test("Prueba3: primer elem es 1", cola_ver_primero(cola3)==&numero1);
    print_test("Prueba3: encolar letra", cola_encolar(cola3, &letra));
    print_test("Prueba3: primer elem es 1", cola_ver_primero(cola3)==&numero1);
    //Comienzo a desencolar uno a uno viendo el primer elemento y corroborando si la cola esta vacia
    print_test("Prueba3: desencolo 1", cola_desencolar(cola3) == &numero1);
    print_test("Prueba3: primer elem es cadena", cola_ver_primero(cola3)==&cadena);
    print_test("Prueba3: la cola no esta vacia", !cola_esta_vacia(cola3));
    print_test("Prueba3: desencolo cadena", cola_desencolar(cola3) == &cadena);
    print_test("Prueba3: primer elem es letra", cola_ver_primero(cola3)==&letra);
    print_test("Prueba3: la cola no esta vacia", !cola_esta_vacia(cola3));
    print_test("Prueba3: desencolo letra", cola_desencolar(cola3) == &letra);
    print_test("Prueba3: no hay elementos", cola_ver_primero(cola3)==NULL);
    print_test("Prueba3: la cola esta vacia", cola_esta_vacia(cola3));
    //intento desencolar en la cola vacia
    print_test("Prueba3: desencolo en cola vacia", cola_desencolar(cola3) == NULL);
    //destruyo la cola
    cola_destruir(cola3, NULL);
}

//Prueba4: encolo, encolo, desencolo y vuelvo a encolar
static void prueba_cola_encolar_desencolar(void) {
    cola_t *cola4 = cola_crear();
    int numero2 = 2;
    char cadena[] = "Chau";
    char letra = 'b';
    //Encolo dos elementos y veo el primer elemento en cada momento
    print_test("Prueba4: encolar 2", cola_encolar(cola4, &numero2)); 
    print_test("Prueba4: primer elem es 2", cola_ver_primero(cola4)==&numero2);
    print_test("Prueba4: encolar cadena", cola_encolar(cola4, &cadena));
    print_test("Prueba4: primer elem es 2", cola_ver_primero(cola4)==&numero2);
    print_test("Prueba4: desencolo 2", cola_desencolar(cola4) == &numero2);
    print_test("Prueba4: primer elem es cadena", cola_ver_primero(cola4)==&cadena);
    print_test("Prueba4: la cola no esta vacia", !cola_esta_vacia(cola4));
    print_test("Prueba4: encolar letra", cola_encolar(cola4, &letra)); 
    print_test("Prueba4: primer elem es cadena", cola_ver_primero(cola4)==&cadena);
    print_test("Prueba4: desencolo cadena", cola_desencolar(cola4) == &cadena);
    print_test("Prueba4: primer elem es letra", cola_ver_primero(cola4)==&letra);
    print_test("Prueba4: la cola no esta vacia", !cola_esta_vacia(cola4));
    print_test("Prueba4: desencolo letra", cola_desencolar(cola4) == &letra);
    print_test("Prueba4: no hay elementos", cola_ver_primero(cola4)==NULL);
    print_test("Prueba4: la cola esta vacia", cola_esta_vacia(cola4));
    //intento desencolar en la cola vacia
    print_test("Prueba4: desencolo en cola vacia", cola_desencolar(cola4) == NULL);
    //destruyo la cola
    cola_destruir(cola4, NULL);
}

//Prueba5: Prueba de volumen
static void prueba_cola_volumen(void) {
    cola_t *cola5 = cola_crear();
    int arreglo[100];
    for (int i =0; i<100; i++) {
        arreglo[i] = i;
    }
    for (int i =0; i<100; i++) {
        cola_encolar(cola5, &arreglo[i]);
    }

    for (int i =0; i<100; i++) {
        print_test("Prueba5: desencolo", cola_desencolar(cola5) == &arreglo[i]);
    }
    //intento desencolar en la cola vacia
    print_test("Prueba5: desencolo en cola vacia", cola_desencolar(cola5) == NULL);
    //destruyo la cola
    cola_destruir(cola5, NULL);
}

// Prueba6: encolo un elemento NULL
static void prueba_cola_encolar_null(void) {
    cola_t *cola6 = cola_crear();
        print_test("Prueba6: encolar elemento nulo", cola_encolar(cola6, NULL)); 
        print_test("Prueba6: primer elem es NULL", cola_ver_primero(cola6)==NULL);
        print_test("Prueba6: la cola no esta vacia", !cola_esta_vacia(cola6));
        print_test("Prueba6: desencolo NULL", cola_desencolar(cola6) == NULL);
        print_test("Prueba6: la cola esta vacia", cola_esta_vacia(cola6));
    //destruyo la cola
    cola_destruir(cola6, NULL);
}

// Prueba7: destruyo una cola con elementos dentro
static void prueba_destruir_con_elementos(void) {
    cola_t *cola7 = cola_crear();
    int numero5= 5;
        print_test("Prueba7: encolar 5", cola_encolar(cola7, &numero5)); 
        print_test("Prueba7: encolar 5", cola_encolar(cola7, &numero5)); 
        print_test("Prueba7: la cola no esta vacia", !cola_esta_vacia(cola7));
    //destruyo la cola
    cola_destruir(cola7, NULL);
}

//Prueba8: llamo a la funcion cola_destruir con una funcion como parámetro para que destruya a los elementos encolados
typedef struct persona {
    char* nombre;
    char* apellido;
    int edad;
} persona_t;
persona_t* persona_crear(char* nombre, char* apellido, int edad) {
    persona_t *persona = malloc(sizeof(persona_t));
    if (!persona) return NULL;
    persona->nombre = nombre;
    persona->apellido = apellido;
    persona->edad = edad;
    return persona;
}
void persona_destruir(void* persona) {
    free(persona);
}
static void prueba_destruir_elementos_persona(void) {
    cola_t *cola8 = cola_crear();
    persona_t *persona1 = persona_crear("Ian", "von der Heyde", 21);
    persona_t *persona2 = persona_crear("Juan", "Perez", 30);
        print_test("Prueba8: encolar persona1", cola_encolar(cola8, persona1)); /////////ATENCION al encolar (como la persona ya es un puntero) NO debo poner "&"
        print_test("Prueba8: encolar persona2", cola_encolar(cola8, persona2)); /////////ATENCION al encolar (como la persona ya es un puntero) NO debo poner "&"
        print_test("Prueba8: la cola no esta vacia", !cola_esta_vacia(cola8));
    cola_destruir(cola8, persona_destruir);
}

//Prueba9: llamo a la funcion cola_destruir con mi pila del TDA PILA
void pila_destruir_wrapper(void* pila) {
    pila_destruir((pila_t *)pila); //convierto un generico a un puntero de tipo pila_t*
}
static void prueba_destruir_elementos_pila(void) {
    cola_t *cola9 = cola_crear();
    pila_t *pila1 = pila_crear();
    int numero1 = 1;
    pila_apilar(pila1, &numero1);
    pila_t *pila2 = pila_crear();
        print_test("Prueba9: encolar pila1", cola_encolar(cola9, pila1)); /////////ATENCION al encolar (como la pila ya es un puntero) NO debo poner "&"
        print_test("Prueba9: encolar pila2", cola_encolar(cola9, pila2)); /////////ATENCION al encolar (como la pila ya es un puntero) NO debo poner "&"
        print_test("Prueba9: la cola no esta vacia", !cola_esta_vacia(cola9));
    cola_destruir(cola9, pila_destruir_wrapper);
}



void pruebas_cola_estudiante() {
    prueba_cola_vacia();
    prueba_cola_un_elemento();
    prueba_cola_varios_elementos();
    prueba_cola_encolar_desencolar();
    prueba_cola_volumen();
    prueba_cola_encolar_null();
    prueba_destruir_con_elementos();
    prueba_destruir_elementos_persona();
    prueba_destruir_elementos_pila();
}

/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_cola_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif

