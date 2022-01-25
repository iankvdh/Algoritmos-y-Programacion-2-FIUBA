#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "usuario.h"

//////////////////////////
//      ALGOGRAM        //
//////////////////////////

typedef struct algogram algogram_t;

/*
    Post: se creo un algogram
*/
algogram_t* algogram_crear(void);

/*
    Pre: algogram existe
    Post: se destuyo el algogram
*/
void algogram_destruir(algogram_t *algogram);

/*
    Carga los usuarios del archivo recibido a un algogram
    Pre: algogram existe
    Post: algogram ahora tiene lo usuarios cargados dentro
*/
bool algogram_cargar_usuarios(algogram_t* algogram, char* ruta_archivo);

/*
    Loggea al usuario del algogram
    Pre: algogram existe
    Post: hay un usuario loggeado en algogram
*/
bool algogram_login(algogram_t* algogram);

/*
    Complejidad: O(u log p)
    Permite al usuario loggeado publicar un nuevo post
    Pre: algogram existe
*/
bool algogram_publicar(algogram_t* algogram);

/*
    Complejidad: O(log p)
    Muestra el proximo post del feed del usuario
    loggeado
    Pre: algogram existe
*/
void algogram_ver_siguiente_feed(algogram_t* algogram);

/*
    Complejidad: O(u)
    Pide a la entrada estandar un post y lo likea
    Pre: algogram existe
*/
bool algogram_likear(algogram_t* algogram);

/*
    Complejidad: O(u)
    Pide a la entrada estandar un post y muestra los
    usuarios que le dieron like
    Pre: algogram existe
*/
bool algogram_mostrar_likes(algogram_t* algogram);

/*
    Desloggea al usuario del algogram
    Pre: algogram existe
    Post: no hay un usuario loggeado en algogram
*/
bool algogram_logout(algogram_t* algogram);
