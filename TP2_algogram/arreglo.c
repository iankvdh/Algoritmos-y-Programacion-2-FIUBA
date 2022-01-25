#include "arreglo.h"

#define FACTOR_REDIMENSION 4
#define TAMANIO_INICIAL 10

struct arreglo {
    size_t tam;
    void **datos;
};

void arreglo_destruir(arreglo_t *arreglo) {
    free(arreglo->datos);
    free(arreglo);
}

void* arreglo_obtener(arreglo_t *arreglo, size_t pos) {
    if (pos >= arreglo->tam) {
        return NULL;
    }
    return arreglo->datos[pos];
}

bool arreglo_guardar(arreglo_t *arreglo, size_t pos, void* valor) {
    if (pos >= arreglo->tam) {
        if (!arreglo_redimensionar(arreglo, arreglo->tam * FACTOR_REDIMENSION)) {
            return false;
        }
    }
    arreglo->datos[pos] = valor;
    return true;
}

size_t arreglo_largo(arreglo_t *arreglo) {
    return arreglo->tam;
}

arreglo_t *arreglo_crear() {
    arreglo_t *arreglo = calloc(1, sizeof(arreglo_t));
    if (arreglo == NULL) {
        return NULL;
    }
    arreglo->datos = calloc(1, TAMANIO_INICIAL * sizeof(void*));
    if (arreglo->datos == NULL) {
        free(arreglo);
        return NULL;
    }
    arreglo->tam = TAMANIO_INICIAL;
    return arreglo;
}

bool arreglo_redimensionar(arreglo_t *arreglo, size_t tam_nuevo) {
    void *datos_nuevo = realloc(arreglo->datos, tam_nuevo * sizeof(void*));
    if (tam_nuevo > 0 && datos_nuevo == NULL) {
        return false;
    }
    arreglo->datos = datos_nuevo;
    arreglo->tam = tam_nuevo;
    return true;
}


void arreglo_destruir_con_datos(arreglo_t *arreglo, void (*destruir_dato)(void *)) {
    for (size_t i = 0; i < arreglo_largo(arreglo); i++) {
        if (arreglo_obtener(arreglo, i)) {
            destruir_dato(arreglo_obtener(arreglo, i));
        }
    }
    free(arreglo->datos);
    free(arreglo);
}