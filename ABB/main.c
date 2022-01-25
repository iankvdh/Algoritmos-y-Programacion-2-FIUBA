#include "testing.h"

#include <stdio.h>
#include <stdlib.h>

/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/

void pruebas_abb_estudiante(void);
//void pruebas_volumen_estudiante(size_t);

int main(int argc, char *argv[]) {
//#ifdef CORRECTOR 


    printf("\n~~~ PRUEBAS CÁTEDRA ~~~\n");
    pruebas_abb_estudiante();
//#endif 

    return failure_count() > 0;
}
