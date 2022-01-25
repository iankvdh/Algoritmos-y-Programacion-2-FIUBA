#include <stdlib.h>
#include <stdbool.h>


/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct abb abb_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);


/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/
/*
 * Crea un ABB vacío. Devuelve NULL en caso de error.
 */
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);
/* Guarda un elemento en el ABB, si la clave ya se encuentra en la
 * estructura, la reemplaza. De no poder guardarlo devuelve false.
 * Pre: La estructura ABB fue inicializada
 * Post: Se almacenó el par (clave, dato)
 */
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);
/* Borra un elemento del ABB y devuelve el dato asociado.  Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura ABB fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void *abb_borrar(abb_t *arbol, const char *clave);
/* Obtiene el valor de un elemento del ABB, si la clave no se encuentra
 * devuelve NULL.
 * Pre: La estructura ABB fue inicializada
 */
void *abb_obtener(const abb_t *arbol, const char *clave);
/* Determina si clave pertenece o no al ABB.
 * Pre: La estructura ABB fue inicializada
 */
bool abb_pertenece(const abb_t *arbol, const char *clave);
/* Devuelve la cantidad de elementos del ABB.
 * Pre: La estructura ABB fue inicializada
 */
size_t abb_cantidad(const abb_t *arbol);
/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * destruir para cada par (clave, dato).
 * Pre: La estructura ABB fue inicializada
 * Post: La estructura ABB fue destruida
 */
void abb_destruir(abb_t *arbol);


/* ******************************************************************
 *                   PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/
// Para cada clave del ABB, alplica la funcion visitar en su dato mientras que la misma devuelva TRUE.
// Si visitar devuelve FALSE, se deja de iterar.
// Pre: existe un ABB, y visitar es una funcion capaz de utilizar o modificar
// los datos del mismo.
                                            //CLAVE        DATO    EXTRA
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);


/* ******************************************************************
 *                   PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

typedef struct abb_iter abb_iter_t;

// Crea iterador
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);
// Avanza iterador
bool abb_iter_in_avanzar(abb_iter_t *iter);
// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);
// Comprueba si terminó la iteración
bool abb_iter_in_al_final(const abb_iter_t *iter);
// Destruye iterador
void abb_iter_in_destruir(abb_iter_t* iter);
