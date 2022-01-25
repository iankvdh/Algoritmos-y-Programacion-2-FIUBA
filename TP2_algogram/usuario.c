#include "usuario.h"
#include "heap.h"
#include <string.h>

//////////////////////////
//      USUARIOS        //
//////////////////////////

typedef struct post_feed post_feed_t;

struct post_feed {
    size_t id_duenio_del_heap;
    post_t* post;
};

struct usuario {
    size_t id;
    char* nombre;
    heap_t* feed; // heap de post_feed_t
};

size_t _distancia(size_t id, size_t act);
int _funcion_comparacion(const void* post_feed_1, const void* post_feed_2);

usuario_t* usuario_crear(size_t id, char* nombre) {
    usuario_t* usuario = calloc(1, sizeof(usuario_t));
    if (!usuario) {
        return NULL;
    }
    usuario->nombre = malloc((strlen(nombre)+1) * sizeof(char));
    if (!usuario->nombre) {
        free(usuario);
        return NULL;
    }
    usuario->feed = heap_crear(_funcion_comparacion);
    if (!usuario->feed) {
        free(usuario->nombre);
        free(usuario);
        return NULL;
    }
    strcpy(usuario->nombre, nombre);
    usuario->id = id;
    return usuario;
}

void usuario_destruir(void* usuario1) {
    usuario_t* usuario = (usuario_t*)usuario1;
    free(usuario->nombre);
    heap_destruir(usuario->feed, free);
    free(usuario);
}

// O(log p)
void usuario_agregar_feed(usuario_t* usuario, post_t* post) {
    post_feed_t* post_a_guardar = calloc(1, sizeof(post_feed_t));
    post_a_guardar->id_duenio_del_heap = usuario->id;
    post_a_guardar->post = post;
    heap_encolar(usuario->feed, post_a_guardar);
}

post_t* usuario_ver_siguiente_feed(usuario_t* usuario) {
    post_feed_t* post_feed = ((post_feed_t*)heap_desencolar(usuario->feed));
    if (!post_feed) {
        return NULL;
    } 
    post_t* post = post_feed->post;
    free(post_feed);
    return post;
}

char* usuario_ver_nombre(usuario_t* usuario) {
    return usuario->nombre;
}

size_t usuario_ver_id(usuario_t* usuario) {
    return usuario->id;
}

//////////////////////////
//  FUNCION COMPARACION //
//////////////////////////
size_t _distancia(size_t id, size_t act) {
    int n = (int)act - (int)id;
    return n < 0 ? (size_t)(-n) : (size_t)n;
}

int _funcion_comparacion(const void* post_feed_1, const void* post_feed_2) {
    size_t id_usuario_1 = post_ver_id_creador(((post_feed_t*)post_feed_1)->post);
    size_t id_usuario_2 = post_ver_id_creador(((post_feed_t*)post_feed_2)->post);
    size_t id_usuario_ref = ((post_feed_t*)post_feed_1)->id_duenio_del_heap;

    size_t id_post1 = post_ver_id(((post_feed_t*)post_feed_1)->post);
    size_t id_post2 = post_ver_id(((post_feed_t*)post_feed_2)->post);

    if (id_usuario_1 == id_usuario_2 || _distancia(id_usuario_1, id_usuario_ref) == _distancia(id_usuario_2, id_usuario_ref)) {
        if (id_post1 > id_post2) {
            return -1;
        }
        return 1;
    }
    if (_distancia(id_usuario_1, id_usuario_ref) > _distancia(id_usuario_2, id_usuario_ref)){
        return -1;
    }
    return 1;
}
