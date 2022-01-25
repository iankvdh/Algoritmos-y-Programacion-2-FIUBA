#include <stdlib.h>
#include <stdbool.h>
#include "post.h"

////////////////////////
//      USUARIO       //
////////////////////////

typedef struct usuario usuario_t;

/*
    Pre: recibe un id y un nombre
    Post: se creo un usuario
*/
usuario_t* usuario_crear(size_t id, char* nombre);

/*
    Pre: el usuario existe
    Post: se destuyo el usuario
*/
void usuario_destruir(void* usuario1);

/*
    Complejidad: O(log p)
    Agrega un post al feed del usuario
    Pre: el usuario y el post existen
    Post: se agrego el post al feed del usuario
*/
void usuario_agregar_feed(usuario_t* usuario, post_t* post);

/*
    Complejidad: O(log p)
    Devuelve siguiente post que el usuario debe ver
    Pre: el usuario existe
    Post: se recibe el post que el usuario debe ver
*/
post_t* usuario_ver_siguiente_feed(usuario_t* usuario);

/*
    Complejidad: O(1)
    Devuelve el nombre del usuario recibido
    Pre: el usuario existe
    Post: se recibe el nombre de ese usuario
*/
char* usuario_ver_nombre(usuario_t* usuario);

/* 
    Complejidad: O(1)
    Devuelve la id del usuario recibido
    Pre: el usuario existe
    Post: se recibe la id de ese usuario
*/
size_t usuario_ver_id(usuario_t* usuario);
