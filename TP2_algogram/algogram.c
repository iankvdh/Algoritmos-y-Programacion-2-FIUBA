#include "algogram.h"
#include "heap.h"
#include "hash.h"
#include "arreglo.h"

/*
gcc -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -o pruebas *.c
valgreen --leak-check=full --track-origins=yes --show-reachable=yes ./pruebas usuarios.txt

./pruebas.sh /home/ian/Documentos/TP2/pruebas
*/


//////////////////////////
//      ALGOGRAM        //
//////////////////////////

struct algogram {
    usuario_t* usuario_loggeado;
    size_t cant_post;
    arreglo_t* posts; // TOODOS LOS POSTS pos: id de post, valor: post_t
    hash_t* usuarios; // clave = nombre valor = usuario_t
    void** nombres_usuarios; //lista con todos los nombres de usuarios ordenados alfabeticamente 
};

algogram_t* algogram_crear(void) {
    algogram_t* algogram = calloc(1, sizeof(algogram_t));
    if (!algogram) {
        return NULL;
    }
    algogram->posts = arreglo_crear();
    if (!algogram->posts) {
        free(algogram);
        return NULL;
    }
    algogram->usuarios = hash_crear(usuario_destruir);
    if (!algogram->usuarios) {
        free(algogram->posts);
        free(algogram);
        return NULL;
    }
    return algogram;
}

void algogram_destruir(algogram_t *algogram) {
    free(algogram->nombres_usuarios);
    arreglo_destruir_con_datos(algogram->posts, post_destruir);
    hash_destruir(algogram->usuarios);
    free(algogram);
}

int _comparar_cadenas(const void* s1, const void* s2) {
    return strcmp((char*)s1, (char*)s2);
}

bool algogram_cargar_usuarios(algogram_t* algogram, char* ruta_archivo) {
    FILE* archivo = fopen(ruta_archivo, "r");
    if (!archivo) {
        return false;
    }
    char * nombre = NULL;
    size_t len = 0;
    ssize_t read;
    // Guardamos en el hash algogram->usuarios todos los usuarios del archivo 
    size_t id_usuario = 0;
    while ((read = getline(&nombre, &len, archivo)) != -1) {
        strtok(nombre, "\n");
        usuario_t* usuario = usuario_crear(id_usuario, nombre);
        hash_guardar(algogram->usuarios, nombre, usuario);
        id_usuario++;
    }
    size_t cantidad_usuarios = hash_cantidad(algogram->usuarios);
    // Guardamos en el arreglo nombres_usuarios todos los nombres de los usuarios del archivo
    // y los ordenamos alfabeticamente 
    algogram->nombres_usuarios = malloc(sizeof(char*)*cantidad_usuarios);
    hash_iter_t* iter = hash_iter_crear(algogram->usuarios);
    size_t n = 0;
    while (!hash_iter_al_final(iter)) {
        algogram->nombres_usuarios[n] = (char*)hash_iter_ver_actual(iter);
        n++;
        hash_iter_avanzar(iter);
    }
    hash_iter_destruir(iter);
    heap_sort(algogram->nombres_usuarios, n, _comparar_cadenas);
    free(nombre);
    fclose(archivo);
    return true;
}

char* _pedir_entrada(void) {
    char* entrada = NULL;
    size_t len = 0;
    if (!getline(&entrada, &len, stdin)) {
        return NULL;
    }
    strtok(entrada, "\n");
    return entrada;
}

// O(1)
bool algogram_login(algogram_t* algogram) {
    if (algogram->usuario_loggeado) {
        fprintf(stdout, "Error: Ya habia un usuario loggeado\n");
        return false;
    }
    char *usuario = _pedir_entrada();
    if (!hash_pertenece(algogram->usuarios, usuario)) {
        fprintf(stdout, "Error: usuario no existente\n"); 
        free(usuario);
        return true;
    }
    algogram->usuario_loggeado = (usuario_t*)hash_obtener(algogram->usuarios, usuario);
    fprintf(stdout, "Hola %s\n", usuario);
    free(usuario);
    return true;
}

// O(u log p)
bool algogram_publicar(algogram_t* algogram) {
    if (!algogram->usuario_loggeado) {
        fprintf(stdout, "Error: no habia usuario loggeado\n");
        return false;
    }
    char* contenido = _pedir_entrada();
    post_t* post = post_crear(algogram->cant_post, contenido, usuario_ver_nombre(algogram->usuario_loggeado), usuario_ver_id(algogram->usuario_loggeado));
    free(contenido);
    if (!post) {
        return false;   
    }
    algogram->cant_post++;
    if (!arreglo_guardar(algogram->posts, post_ver_id(post), post)) {
        return false;
    }
    hash_iter_t* iter_usuarios = hash_iter_crear(algogram->usuarios);
    if (!iter_usuarios) {
        return false;
    }
    while (!hash_iter_al_final(iter_usuarios)) { // O(u)
        usuario_t* usuario = (usuario_t*)hash_obtener(algogram->usuarios, hash_iter_ver_actual(iter_usuarios)); //itero usuarios
        if (usuario != algogram->usuario_loggeado) { // si es el que publico, lo salteo
            usuario_agregar_feed(usuario, post); // O(log p)
        }
        hash_iter_avanzar(iter_usuarios);
    }
    hash_iter_destruir(iter_usuarios);
    fprintf(stdout, "Post publicado\n");
    return true;
}

// O(Log p)
void algogram_ver_siguiente_feed(algogram_t* algogram) {
    if (!algogram->usuario_loggeado) {
        fprintf(stdout, "Usuario no loggeado o no hay mas posts para ver\n"); 
        return;
    }
    post_t* post = usuario_ver_siguiente_feed(algogram->usuario_loggeado);
    if (!post) {
        fprintf(stdout, "Usuario no loggeado o no hay mas posts para ver\n"); 
        return;
    }
    fprintf(stdout, "Post ID %ld\n", post_ver_id(post));
    fprintf(stdout, "%s dijo: %s\n", post_ver_creador(post), post_ver_contenido(post));
    fprintf(stdout, "Likes: %ld\n", post_ver_cant_likes(post));
}

// Es O(1)
post_t* _buscar_post(algogram_t* algogram) {
    char* entrada = _pedir_entrada();
    int pos = atoi(entrada);
    free(entrada);
    if (algogram->cant_post <= pos || pos < 0) {
        return NULL;
    }
    return arreglo_obtener(algogram->posts, (size_t)pos);
}

// O(1) (la pagina dice que deberia ser O(log u))
bool algogram_likear(algogram_t* algogram) {
    if (!algogram->usuario_loggeado) {
        fprintf(stdout, "Error: Usuario no loggeado o Post inexistente\n");
        return false;
    }
    post_t* post = _buscar_post(algogram);
    if (!post) {
        fprintf(stdout, "Error: Usuario no loggeado o Post inexistente\n");
        return false;
    }
    post_likear(post, usuario_ver_nombre(algogram->usuario_loggeado));
    fprintf(stdout, "Post likeado\n");
    return true;
}

// Es O(u) -> u = cantidad de usuarios
bool algogram_mostrar_likes(algogram_t* algogram) {
    post_t* post = _buscar_post(algogram);
    if (!post || !post_ver_cant_likes(post)) {
        fprintf(stdout, "Error: Post inexistente o sin likes\n");
        return false;
    }
    fprintf(stdout, "El post tiene %ld likes:\n", post_ver_cant_likes(post));
    for (size_t i = 0; i < hash_cantidad(algogram->usuarios); i++) { // O(u)
        if (post_dio_like(post, (char*)algogram->nombres_usuarios[i])) {
            fprintf(stdout, "	%s\n", (char*)algogram->nombres_usuarios[i]);
        }
    }
    return true;
}

// O(1)
bool algogram_logout(algogram_t* algogram) {
    if (!algogram->usuario_loggeado) {
        fprintf(stdout, "Error: no habia usuario loggeado\n");
        return false;
    }
    algogram->usuario_loggeado = NULL;
    fprintf(stdout, "Adios\n");
    return true;
}
