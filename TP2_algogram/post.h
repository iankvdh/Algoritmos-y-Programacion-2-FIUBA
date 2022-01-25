#include <stdlib.h>
#include <stdbool.h>

///////////////////////
//       POST        //
///////////////////////

typedef struct post post_t;

/*
    Pre: recibe un id, un contenido, y el nombre e id de un usuario
    Post: se creo un post
*/
post_t* post_crear(size_t id, char* contenido, char* creador, size_t id_creador);

/*
    Pre: el post existe
    Post: se destuyo el post
*/
void post_destruir(void* posteo);

/*
    Complejidad O(1)
    Pre: el post existe
    Post: el post tiene un like nuevo
*/
void post_likear(post_t* post, char* nombre_usuario);

/*
    Pre: el post existe
    Post: se consiguio el id de ese post
*/
size_t post_ver_id(post_t* post);
 
/*
    Pre: el post existe
    Post: se consiguio la cantidad de likes de ese post
*/
size_t post_ver_cant_likes(post_t* post);

/*
    Pre: el post existe
    Post: se consiguio el contenido de ese post
*/
char* post_ver_contenido(post_t* post);

/*
    Pre: el post existe
    Post: se consiguio el nombre del creador de ese post
*/
char* post_ver_creador(post_t* post);

/*
    Pre: el post existe
    Post: se consiguio el id del creador de ese post
*/
size_t post_ver_id_creador(post_t* post);

/*  
    Complejidad O(1)
    Retorna true si el usuario recibido dio like
    al post, false en caso contrario
    Pre: el post existe
*/
bool post_dio_like(post_t* post, char* nombre_usuario);
