#include <stdbool.h>
#include <stdlib.h>

/*******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 ******************************************************************/

typedef struct arreglo arreglo_t;


/*******************************************************************
 *                    PRIMITIVAS DEL arreglo
 ******************************************************************/

// Crea un arreglo de tamaño TAMANIO INICIAL
// Post: arreglo es una arreglo vacío de tamaño TAMANIO INICIAL
arreglo_t *arreglo_crear();

// Destruye el arreglo
// Pre: el arreglo fue creado
// Post: se eliminaron todos los elementos del arreglo
void arreglo_destruir(arreglo_t *arreglo);

// Cambia el tamaño del arreglo
// Pre: el arreglo fue creado
// Post: el arreglo cambió de tamaño a nuevo_tam y devuelve true
// o el arreglo queda intacto y devuelve false si no se pudo cambiar el tamaño
// a nuevo_tam
bool arreglo_redimensionar(arreglo_t *arreglo, size_t nuevo_tam);

// Devuelve el dato de la posicion dada
// Pre: el arreglo fue creado
// Post: se devolvió el dato almacenado. Devuelve NULL si la
// posición es inválida (fuera del rango del arreglo, que va de 0 a tamaño-1)
void* arreglo_obtener(arreglo_t *arreglo, size_t pos);

// Almacena el valor en la posición pos
// Pre: el arreglo fue creado
// Post: se almacenó el valor en la posición pos. Devuelve false si la posición
// es inválida (fuera del rango del arreglo, que va de 0 a tamaño-1) y true si
// se guardó el valor con éxito.
bool arreglo_guardar(arreglo_t *arreglo, size_t pos, void* valor);

// Devuelve el tamaño del arreglo
// Pre: el arreglo fue creado
size_t arreglo_largo(arreglo_t *arreglo);

// Destruye el arreglo y sus datos
// Pre: el arreglo fue creado
// Post: se eliminaron todos los elementos del arreglo
void arreglo_destruir_con_datos(arreglo_t *arreglo, void (*destruir_dato)(void *));


