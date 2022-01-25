#include "algogram.h"

bool ordenes(algogram_t* algogram) {
    char * funcion = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&funcion, &len, stdin)) != -1) {
        if (strcmp(funcion, "login\n") == 0) {
            algogram_login(algogram);
            free(funcion);
            return true;
        }
        if (strcmp(funcion, "publicar\n") == 0) {
            algogram_publicar(algogram);
            free(funcion);
            return true;
        }       
        if (strcmp(funcion, "ver_siguiente_feed\n") == 0) {
            algogram_ver_siguiente_feed(algogram);
            free(funcion);
            return true;
        }
        if (strcmp(funcion, "likear_post\n") == 0) {
            algogram_likear(algogram);
            free(funcion);
            return true;
        }
        if (strcmp(funcion, "mostrar_likes\n") == 0) {
            algogram_mostrar_likes(algogram);
            free(funcion);
            return true;
        }
        if (strcmp(funcion, "logout\n") == 0) {
            algogram_logout(algogram);
            free(funcion);
            return true;
        }
    }
    free(funcion);
    return false;
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        return -1;
    }

    algogram_t* algogram = algogram_crear();
    if (!algogram) {
        return -1;
    }
    
    if (!algogram_cargar_usuarios(algogram, argv[1])) {
        algogram_destruir(algogram);
        return -1;
    }

    while (true) {
        if(!ordenes(algogram)) {
            break;
        }
    }
    algogram_destruir(algogram);
    return 0;
}
