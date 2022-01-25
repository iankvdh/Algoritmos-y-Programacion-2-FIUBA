#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "hash.h"

#define TAMANIO_INICIAL 7
#define FACTOR_REDIMENSION 0.7
#define FACTOR_AGRANDAR 2.0
#define FACTOR_ACHICAR 0.5

typedef enum {VACIO = 0, BORRADO, OCUPADO} estado_t;

typedef struct campo {
    char* clave;
    void* dato;
    estado_t estado;
} campo_t;

campo_t* campo_crear(void);

hash_t *_hash_inicializar(hash_destruir_dato_t destruir_dato, size_t tamanio);

bool hay_que_redimensionar_agrandar(hash_t *hash);

bool hay_que_redimensionar_achicar(hash_t *hash);

unsigned long funcion_hash(const char *str, size_t largo);

void _hash_redimensionar(hash_t* hash, double factor);

void destruir_tabla(campo_t** tabla, size_t largo);

//Recibe una clave y un hash y devuelve la posicion del hash en la que se encuentra la clave. Devuelve -1 en caso de que la clave no esté en el hash
int hash_buscar (const hash_t *hash, const char *clave);

//Recibe un hash_iter y una posicion y devuelve la siguiente posicion ocupada del hash en caso de haberla. En caso contrario devuelve -1
int hash_avanzar_a_ocupado(hash_iter_t *iter, int pos);

struct hash {
    campo_t** tabla;
    hash_destruir_dato_t destruir_dato;
    size_t largo;
    size_t ocupados;
    size_t borrados;
};

hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
    return _hash_inicializar(destruir_dato, TAMANIO_INICIAL);
}

hash_t *_hash_inicializar(hash_destruir_dato_t destruir_dato, size_t tamanio) {
    // Crear hash
    hash_t *hash = calloc(1, sizeof(hash_t));
    if (!hash) {
        return NULL;
    }
    // Crear tabla
    hash->tabla = malloc(tamanio * sizeof(campo_t*));
    if (!hash->tabla) {
        free(hash);
        return NULL;
    }
    // Asignar campos
    for (size_t i = 0; i < tamanio; i++) {
        hash->tabla[i] = campo_crear();
        if (!hash->tabla[i]) {
            destruir_tabla(hash->tabla, i-1);
            free(hash);
            return NULL;
        }
    }
    // Asignar valores iniciales
    hash->largo = tamanio;
    hash->destruir_dato = destruir_dato;

    return hash;
}

campo_t* campo_crear(void) {
    campo_t* nuevo_campo = calloc(1, sizeof(campo_t));
    if (!nuevo_campo) {
        return NULL;
    }
    return nuevo_campo;
}

bool hay_que_redimensionar_agrandar(hash_t *hash) {
    return ((double)hash->ocupados + (double)hash->borrados) / (double)hash->largo >= FACTOR_REDIMENSION;
}

bool hay_que_redimensionar_achicar(hash_t *hash) {
    if (hash->largo <= TAMANIO_INICIAL) {
        return false;
    }
    return ((double)hash->ocupados + (double)hash->borrados) / (double)hash->largo <= 0.1;
}

//djb2
unsigned long funcion_hash(const char *str, size_t largo) {
    //https://stackoverflow.com/questions/7666509/hash-function-for-string
    //http://www.cse.yorku.ca/~oz/hash.html
    unsigned long hash = 5381;
    unsigned long c;
    while ((c = (unsigned long)*str++))
        hash = ((hash << 5) + hash) + c;
    return hash % largo;
}

void _hash_redimensionar(hash_t* hash, double factor) {
    hash_t* hash_aux = _hash_inicializar(hash->destruir_dato, (size_t)((double)hash->largo * factor));
    
    for (int i = 0; i < hash->largo; i++) {
        if (hash->tabla[i]->estado == OCUPADO) {
            hash_guardar(hash_aux, hash->tabla[i]->clave, hash->tabla[i]->dato);
        }
    }
    destruir_tabla(hash->tabla, hash->largo);
    hash->tabla = hash_aux->tabla;
    hash->largo = hash_aux->largo;
    free(hash_aux);
}

void hash_destruir(hash_t *hash) {
    for (size_t i = 0; i < hash->largo; i++) {
        if (hash->destruir_dato && hash->tabla[i]->estado == OCUPADO) {
            hash->destruir_dato(hash->tabla[i]->dato);
        }
        free(hash->tabla[i]->clave);
        free(hash->tabla[i]);
    }
    free(hash->tabla);
    free(hash);
}

void destruir_tabla(campo_t** tabla, size_t largo) {
    for (size_t i = 0; i < largo; i++) {
        free(tabla[i]->clave);
        free(tabla[i]);
    }
    free(tabla);
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
    if (hay_que_redimensionar_agrandar(hash)) {
        _hash_redimensionar(hash, FACTOR_AGRANDAR);
        if (!hash) {
           return false;    
        }
    }
    int pos = hash_buscar(hash, clave);
    if (pos != -1 && hash->destruir_dato) { //Si es ocupado y las claves son iguales libero antes de pisar.
        hash->destruir_dato(hash->tabla[pos]->dato);
        hash->tabla[pos]->dato = dato;
        return true;
    }
    else {
        pos = (int)funcion_hash(clave, hash->largo);
        while (hash->tabla[pos]->estado == OCUPADO) {
            pos++;
            pos = pos % (int)hash->largo; // pos %= hash->largo
        }
    }
    
    if (hash->tabla[pos]->estado != OCUPADO) {
        hash->ocupados++;
    }
    if (hash->tabla[pos]->estado == BORRADO) {
        hash->borrados--;
    }
    hash->tabla[pos]->estado = OCUPADO;
    hash->tabla[pos]->dato = dato;
    free(hash->tabla[pos]->clave); 
    hash->tabla[pos]->clave = malloc(strlen (clave) + 1);
    strcpy(hash->tabla[pos]->clave, clave);

    return true;
}

int hash_buscar (const hash_t *hash, const char *clave) {
    size_t pos = funcion_hash(clave, hash->largo);
    while (hash->tabla[pos]->estado != VACIO) {
        if (hash->tabla[pos]->clave && strcmp(clave, hash->tabla[pos]->clave) == 0) {
            return (int)pos;
        }
        pos++;
        pos = pos % hash->largo; // pos %= hash->largo
    }
    return -1;
}

bool hash_pertenece(const hash_t *hash, const char *clave) {
    return hash_buscar(hash, clave) != -1;
}

size_t hash_cantidad(const hash_t *hash) {
    return hash->ocupados;
}

void *hash_obtener(const hash_t *hash, const char *clave) {
    int pos = hash_buscar(hash, clave);
    return pos != -1? hash->tabla[pos]->dato : NULL;
}

void *hash_borrar(hash_t *hash, const char *clave) {
    int pos = hash_buscar(hash, clave);
    if (pos == -1) {
        return NULL;
    }
    void* dato = hash->tabla[pos]->dato;
    hash->tabla[pos]->estado = BORRADO;
    free(hash->tabla[pos]->clave);
    hash->tabla[pos]->clave = NULL;
    hash->ocupados--;
    hash->borrados++;

    if (hay_que_redimensionar_achicar(hash)) {
        _hash_redimensionar(hash, FACTOR_ACHICAR);
           //return; QUE DEBERIA DEVOLVER
    }
    return dato;
}

///////////////////
//   ITERADOR    //
///////////////////

struct hash_iter {
    void* ant;
    int act;
    const hash_t* hash;
};

int hash_avanzar_a_ocupado(hash_iter_t *iter, int pos) {
    while (pos != iter->hash->largo) {
        if (iter->hash->tabla[pos]->estado != OCUPADO) {
            pos++;
            continue;
        }
            return pos;
    }
    return -1;
}

hash_iter_t *hash_iter_crear(const hash_t *hash) {
    hash_iter_t* iter = calloc(1, sizeof(hash_iter_t));
    if (!iter) {
        return NULL;
    }
    iter->ant = NULL;

    iter->hash = hash;
    iter->act = hash_avanzar_a_ocupado(iter, 0);
    
    return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter) {
    if (hash_iter_al_final(iter)) {
        return false;
    }
    iter->act = hash_avanzar_a_ocupado(iter, iter->act + 1);
    return true;
}

bool hash_iter_al_final(const hash_iter_t *iter) {
    return iter->act == -1;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter) {
    if (hash_iter_al_final(iter)) {
        return NULL;
    }
    return iter->hash->tabla[iter->act]->clave;
}

void hash_iter_destruir(hash_iter_t *iter) {
    free(iter);
}
