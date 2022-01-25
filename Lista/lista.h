#include <stdbool.h>
#include <stdlib.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* La lista está planteada como una lista de punteros genéricos. */

typedef struct lista lista_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista
// Post: Devuelve una nueva lista vacia
lista_t *lista_crear(void);

// Retorna true si la lista esta vacia, false en caso contrario
// Pre: la lista fue creada
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento en la primer posicion de la lista. 
// Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento que contiene a valor
// en la primer posicion de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un nuevo elemento en la ultima posicion de la lista. 
// Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento que contiene a valor
// en la ultima posicion de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Saca el primer elemento de la lista. Si la lista tiene elementos, se quita el
// primero de la lista, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del primer elemento anterior, la lista
// contiene un elemento menos, si la lista no estaba vacía.
void *lista_borrar_primero(lista_t *lista);

// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void *lista_ver_primero(const lista_t *lista);

// Obtiene el valor del ultimo elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del ultimo, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el ultimo elemento de la lista, cuando no está vacía.
void *lista_ver_ultimo(const lista_t* lista);

// Obtiene el largo de la lista.
// Pre: La lista fue creada.
// Post: Se devolvio la cantidad de elementos insertados en la lista. 
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));

/* ******************************************************************
 *                   PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

// Para cada elemento de la lista, la recorre llamando a la funcion visitar
// para cada elemento insertado mientras que visitar devuelva verdadero. Si
// visitar devuelve falso, se deja de iterar.
// Pre: existe una lista, y visitar es una funcion capaz de utilizar o modificar
// los datos de la misma.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

/* ******************************************************************
 *                   PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

typedef struct lista_iter lista_iter_t;

// Crea un iterador externo para una lista y se posiciona
// en la posicion del primer elemento.
// Pre: la lista fue creada
// Post: Retorna un iterador que puede recorrer la lista que fue dada
lista_iter_t *lista_iter_crear(lista_t *lista);

// Avanza la posicion del iterador al siguiente elemento del que
// este parado. En caso de estar al final devuelve false. En caso
// contrario devuelve true.
// Pre: la lista y el iterador fueron creados.
bool lista_iter_avanzar(lista_iter_t *iter);

// Devuelve el valor del elemento que se encuentra en la posicion
// actual donde este parado el iterador. Devuelve NULL si el iterador
// esta al final de la lista.
// Pre: la lista y el iterador fueron creados.
// Post: se obtuvo el valor almacenado en la posicion de la lista
// donde esta el iterador.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Devuelve true si el iterador esta al final de la lista (apuntando a NULL),
// false en caso contrario.
// Pre: la lista y el iterador fueron creados.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador
// Pre: el iterador fue creado
// Post: se destruyo el iterador
void lista_iter_destruir(lista_iter_t *iter);

// Inserta un elemento a la lista en la posicion en donde este
// parado el iterador actualmente.
// Retorna true si lo pudo insertar correctamente, false en caso contrario
// Pre: la lista y el iterador fueron creados.
// Post: a la lista se le inserto un elemento en la posicion
// actual del iterador.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Borra un elemento de la lista en la posicion en donde este
// parado el iterador actualmente y lo devuelve.
// Retorna el elemento si lo pudo borrar correctamente, NULL si el iterador
// estaba al final.
// Pre: la lista y el iterador fueron creados.
// Post: a la lista se le borro un elemento en la posicion
// actual del iterador y se retorno ese elemento
void *lista_iter_borrar(lista_iter_t *iter);
