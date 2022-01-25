#include "post.h"
#include "hash.h"
#include <string.h>

///////////////////////
//       POST        //
///////////////////////

struct post {
    char* creador;
    size_t id_creador;
    size_t id;
    char* contenido;
    size_t cant_likes;
    hash_t* usuarios_like; //clave = nombre de usuario, valor = NULL;
};

post_t* post_crear(size_t id, char* contenido, char* creador, size_t id_creador) {
    post_t* post = calloc(1, sizeof(post_t));
    if (!post) {
        return NULL;
    }
    post->contenido = malloc((strlen(contenido)+1) * sizeof(char));
    if (!post->contenido) {
        free(post);
        return NULL;
    }
    post->usuarios_like = hash_crear(free);
     if (!post->usuarios_like) {
        free(post->contenido);
        free(post);
        return NULL;
    }
    strcpy(post->contenido, contenido);
    post->id = id;
    post->creador = creador;
    post->id_creador = id_creador;
    return post;
}

void post_destruir(void* posteo) {
    if (!posteo) {
        return;
    }
    post_t* post = (post_t*)posteo;
    free(post->contenido);
    hash_destruir(post->usuarios_like);
    free(posteo);
}

void post_likear(post_t* post, char* nombre_usuario) {
    if (!hash_pertenece(post->usuarios_like, nombre_usuario)) {
        hash_guardar(post->usuarios_like, nombre_usuario, NULL);
        post->cant_likes++;
    }
}

size_t post_ver_id(post_t* post) {
    return post->id;
}

size_t post_ver_cant_likes(post_t* post) {
    return post->cant_likes;
}

char* post_ver_contenido(post_t* post) {
    return post->contenido;
}

char* post_ver_creador(post_t* post) {
    return post->creador;
}

size_t post_ver_id_creador(post_t* post) {
    return post->id_creador;
}

bool post_dio_like(post_t* post, char* nombre_usuario) {
    return hash_pertenece(post->usuarios_like, nombre_usuario);
}