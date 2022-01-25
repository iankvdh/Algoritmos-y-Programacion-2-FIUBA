#include <stdbool.h>
#include <stdlib.h>
#include "lista.h"

typedef struct nodo {
    void* dato;
    struct nodo* prox;
} nodo_t;

struct lista {
    nodo_t* prim;
    nodo_t* ult;
    size_t largo;
};

lista_t *lista_crear(void) {
    lista_t* nueva_lista = malloc(sizeof(lista_t));
    if (nueva_lista == NULL) {
        return NULL;
    }
    nueva_lista->largo = 0;
    nueva_lista->prim = NULL;
    nueva_lista->ult = NULL;
    return nueva_lista;
}

bool lista_esta_vacia(const lista_t *lista) {
    return lista->prim == NULL && lista->ult == NULL;
}

bool lista_insertar_primero(lista_t *lista, void *dato) {
    nodo_t* nodo_crear = malloc(sizeof(nodo_t));
    if (!nodo_crear) {
        return false;
    }
    nodo_crear->dato = dato;
    nodo_crear->prox = lista->prim;
    lista->prim = nodo_crear;
    lista->largo++;
    if (lista->largo == 1) {
        lista->ult = lista->prim;
    }
    return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato) {
    nodo_t* nodo_crear = malloc(sizeof(nodo_t));
    if (!nodo_crear) {
        return false;
    }
    nodo_crear->dato = dato;
    if (lista_esta_vacia(lista)) {
        lista->prim = nodo_crear;
    } else {
        lista->ult->prox = nodo_crear;
    }
    lista->ult = nodo_crear;
    nodo_crear->prox = NULL;
    lista->largo++;
    return true;
}

void *lista_borrar_primero(lista_t *lista) { 
    if (lista_esta_vacia(lista)) {
        return NULL;
    }
    nodo_t *nodo_a_borrar = lista->prim;
    void *dato = nodo_a_borrar->dato;
    lista->prim = lista->prim->prox;
    free(nodo_a_borrar);
    lista->largo--;
    if (lista->largo == 0) {
        lista->ult = NULL;
    }
    return dato;
}

void *lista_ver_primero(const lista_t *lista) {
    return lista_esta_vacia(lista) ? NULL : lista->prim->dato;
}

void *lista_ver_ultimo(const lista_t* lista) {
    return lista_esta_vacia(lista) ? NULL : lista->ult->dato;
}

size_t lista_largo(const lista_t *lista) {
    return lista->largo;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)) {
    while (!lista_esta_vacia(lista)) {
        if (destruir_dato != NULL) {
            destruir_dato(lista->prim->dato);   
        }
        lista_borrar_primero(lista);
    }
    free(lista);
    return;
}

// Iterador interno
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra) {
    nodo_t* nodo = lista->prim;
    while (nodo && nodo->dato) {
        if (!visitar(nodo->dato, extra)) {
            break;
        }
        nodo = nodo->prox;
    }
    return;
}

// Iterador externo
struct lista_iter {
    nodo_t* ant;
    nodo_t* act;
    lista_t* lista;
};

lista_iter_t *lista_iter_crear(lista_t *lista) {
    lista_iter_t* iter = malloc(sizeof(lista_iter_t));
    if (!iter) {
        return NULL;
    }
    iter->ant = NULL;
    iter->act = lista->prim;
    iter->lista = lista;
    return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter) {
    if (lista_iter_al_final(iter)) {
        return false;
    }
    iter->ant = iter->act;
    iter->act = iter->act->prox;
    return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter) {
    if (lista_iter_al_final(iter)) {
        return NULL;
    }
    return iter->act->dato;
}
bool lista_iter_al_final(const lista_iter_t *iter) {
    return iter->act == NULL ? true : false;
}

void lista_iter_destruir(lista_iter_t *iter) {
    free(iter);
}
bool lista_iter_insertar(lista_iter_t *iter, void *dato) {
    //caso actual en primero
    if (!iter->ant) {
        if (!lista_insertar_primero(iter->lista, dato)) {
            return false;
        }
        iter->act = iter->lista->prim;
        return true;
    }
    //caso iter en ultimo (NULL)
    if (lista_iter_al_final(iter)) {
        if (!lista_insertar_ultimo(iter->lista, dato)) {
            return false;
        }
        iter->act = iter->lista->ult;
        return true;
    }
    //Caso Feliz
    nodo_t* nodo_crear = malloc(sizeof(nodo_t));
    if (!nodo_crear) {
        return false;
    }
    nodo_crear->dato = dato;
    iter->ant->prox = nodo_crear;
    nodo_crear->prox = iter->act;
    iter->act = nodo_crear;
    iter->lista->largo++;
    return true;
}

void *lista_iter_borrar(lista_iter_t *iter) {
    //Caso borde: iter en primero
    if (!iter->ant) {
        void* dato = lista_borrar_primero(iter->lista);
        if (dato) {
        iter->act = iter->lista->prim;
        }
        return dato;
    }
    // Caso borde: iter en ultimo (NULL)
    if (lista_iter_al_final(iter)) {
        return NULL;
    }
    // Caso feliz
    void* dato = iter->act->dato;
    iter->ant->prox = iter->act->prox;
    free(iter->act);
    iter->act = iter->ant->prox;
    if (!iter->act) {
        iter->lista->ult = iter->ant;
    }
    iter->lista->largo--;
    return dato;
}